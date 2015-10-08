#include "tree_extension.hpp"
#include "core_qt_common/models/adapters/child_list_adapter.hpp"
#include "core_qt_common/models/adapters/indexed_adapter.hpp"
#include "core_data_model/i_item.hpp"
#include "core_variant/variant.hpp"
#include "core_logging/logging.hpp"
#include <QSettings>

struct TreeExtension::Implementation
{
	Implementation( TreeExtension& self );
	~Implementation();
	void expand( const QModelIndex& index );
	void collapse( const QModelIndex& index );
	bool expanded( const QModelIndex& index ) const;
	bool getIndexPath( const QModelIndex& index, std::string & path ) const;
	void saveStates( const char * id );
	void loadStates( const char * id );

	TreeExtension& self_;
	std::vector< IndexedAdapter< ChildListAdapter > > childModels_;
	std::vector< std::unique_ptr< ChildListAdapter > > redundantChildModels_;
	std::vector< std::string > expandedList_;
	std::vector< IItem* >	memoryExpandedList_;

	QModelIndex currentIndex_;
	QSettings settings_;
};

TreeExtension::Implementation::Implementation( TreeExtension & self )
	: self_( self )
	, settings_( "Wargaming.net", "runtime_wg_tree_settings" )
{

}

TreeExtension::Implementation::~Implementation()
{
	memoryExpandedList_.clear();
}

bool TreeExtension::Implementation::getIndexPath( const QModelIndex& index, std::string & path ) const
{
	auto item = reinterpret_cast< IItem * >( index.internalPointer() );
	assert(item != nullptr);
	Variant value = item->getData( 0, IndexPathRole::roleId_ );
	bool isOk = value.tryCast( path );
	if (!isOk || value.isVoid())
	{
		NGT_WARNING_MSG( 
			"Tree preference won't save: %s\n",
			"Please provide an unique path string for IndexPathRole of IItem" );
		return false;
	}
	return true;
}

void TreeExtension::Implementation::expand( const QModelIndex& index )
{
	std::string indexPath("");
	bool hasPath = getIndexPath( index, indexPath );
	if (hasPath)
	{
		if (!expanded( index ))
		{
			expandedList_.push_back( indexPath );
		}
	}
	else
	{
		if (!expanded( index ))
		{
			auto item = reinterpret_cast< IItem * >( index.internalPointer() );
			assert(item != nullptr);
			memoryExpandedList_.push_back( item );
		}
	}
	
}


void TreeExtension::Implementation::collapse( const QModelIndex& index )
{
	std::string path("");
	bool hasPath = getIndexPath( index, path );
	if (hasPath)
	{
		auto it = std::find( expandedList_.begin(), expandedList_.end(), path );
		if (it != expandedList_.end())
		{
			std::swap( 
				expandedList_[ it - expandedList_.begin() ], 
				expandedList_[ expandedList_.size() - 1 ] );
			expandedList_.pop_back();
		}
	}
	else
	{
		auto item = reinterpret_cast< IItem * >( index.internalPointer() );
		assert(item != nullptr);
		auto it = std::find( memoryExpandedList_.begin(), memoryExpandedList_.end(), item );
		if (it != memoryExpandedList_.end())
		{
			std::swap( 
				memoryExpandedList_[ it - memoryExpandedList_.begin() ], 
				memoryExpandedList_[ memoryExpandedList_.size() - 1 ] );
			memoryExpandedList_.pop_back();
		}
	}
}


bool TreeExtension::Implementation::expanded( const QModelIndex& index ) const
{
	if (!index.parent().isValid())
	{
		return true;
	}
	std::string indexPath("");
	bool hasPath = getIndexPath( index, indexPath );
	if (hasPath)
	{
		return 
			std::find( expandedList_.cbegin(), expandedList_.cend(), indexPath ) != 
			expandedList_.cend();
	}
	else
	{
		auto item = reinterpret_cast< IItem * >( index.internalPointer() );
		assert(item != nullptr);
		return 
			std::find( memoryExpandedList_.cbegin(), memoryExpandedList_.cend(), item ) != 
			memoryExpandedList_.cend();
	}
}

void TreeExtension::Implementation::saveStates( const char * id )
{
	if (id == nullptr || id == std::string( "" ))
	{
		NGT_WARNING_MSG( 
			"Tree preference won't save: %s\n", "Please provide unique objectName for WGTreeModel in qml" );
		return;
	}
	QList<QString> list;
	for (auto item : expandedList_)
	{
		list.push_back( item.c_str() );
	}
	settings_.beginWriteArray( id );
	for (int i = 0; i < list.size(); ++i)
	{
		settings_.setArrayIndex( i );
		settings_.setValue( "value", list.at( i ) );
	}
	settings_.endArray();
}

void TreeExtension::Implementation::loadStates( const char * id )
{
	if (id == nullptr || id == std::string( "" ))
	{
		return;
	}
	int size = settings_.beginReadArray( id );
	for (int i = 0; i < size; ++i)
	{
		settings_.setArrayIndex( i );
		QString value = settings_.value( "value" ).toString();
		std::string strValue = value.toUtf8().constData();
		expandedList_.push_back( strValue );
	}
	settings_.endArray();
}


TreeExtension::TreeExtension()
	: impl_( new Implementation( *this ) )
{
}


TreeExtension::~TreeExtension()
{
}

void TreeExtension::saveStates( const char * modelUniqueName )
{
	impl_->saveStates( modelUniqueName );
}

void TreeExtension::loadStates( const char * modelUniqueName )
{
	impl_->loadStates( modelUniqueName );
}


QHash< int, QByteArray > TreeExtension::roleNames() const
{
	QHash< int, QByteArray > roleNames;
	this->registerRole( ChildModelRole::role_, roleNames );
	this->registerRole( HasChildrenRole::role_, roleNames );
	this->registerRole( ExpandedRole::role_, roleNames );
	this->registerRole( ParentIndexRole::role_, roleNames );
	return roleNames;
}


QVariant TreeExtension::data( const QModelIndex &index, int role ) const
{
	size_t roleId;
	if (!this->decodeRole( role, roleId ))
	{
		return QVariant( QVariant::Invalid );
	}

	if (roleId == ChildModelRole::roleId_)
	{
		if (!model_->hasChildren(index) ||
			!impl_->expanded( index ))
		{
			return QVariant( QVariant::Invalid );
		}

		auto it = std::find( impl_->childModels_.begin(), 
			impl_->childModels_.end(), index );
		if (it != impl_->childModels_.end())
		{
			return QVariant::fromValue< QAbstractItemModel* >( it->data_.get() );
		}
		else
		{
			auto pChildModel = new ChildListAdapter( index );
			impl_->childModels_.emplace_back( index, pChildModel );
			return QVariant::fromValue< QAbstractItemModel* >( pChildModel );
		}
	}
	else if (roleId == HasChildrenRole::roleId_)
	{
		return model_->hasChildren( index );
	}
	else if (roleId == ExpandedRole::roleId_)
	{
		return impl_->expanded( index );
	}
	else if (roleId == ParentIndexRole::roleId_)
	{
		QModelIndex parentIndex = model_->parent( index );
		return parentIndex;
	}

	return QVariant( QVariant::Invalid );
}


bool TreeExtension::setData( 
	const QModelIndex &index, const QVariant &value, int role )
{
	size_t roleId;
	if (!this->decodeRole( role, roleId ))
	{
		return false;
	}

	if (roleId == ExpandedRole::roleId_)
	{
		// Change the data
		auto expand = value.toBool();
		if (impl_->expanded( index ) == expand)
		{
			return false;
		}

		expand ? impl_->expand( index ) : impl_->collapse( index );

		// Emit the data change
		QVector< int > roles;
		roles.append( role );
		// The child model role is dependent on the expanded role
		auto res = this->encodeRole( ChildModelRole::roleId_, role );
		assert( res );
		roles.append( role );
		emit model_->dataChanged( index, index, roles );

		return true;
	}

	return false;
}


void TreeExtension::onLayoutAboutToBeChanged(
	const QList<QPersistentModelIndex> & parents, 
	QAbstractItemModel::LayoutChangeHint hint )
{
	for (auto it = parents.begin(); it != parents.end(); ++it)
	{
		isolateRedundantIndices( 
			*it, impl_->childModels_, impl_->redundantChildModels_ );
	}
}


void TreeExtension::onLayoutChanged(
	const QList<QPersistentModelIndex> & parents, 
	QAbstractItemModel::LayoutChangeHint hint )
{
	impl_->redundantChildModels_.clear();

	QVector< int > roles;
	int role;
	auto res = this->encodeRole( ChildModelRole::roleId_, role );
	assert( res );
	roles.append( role );
	res = this->encodeRole( HasChildrenRole::roleId_, role );
	assert( res );
	roles.append( role );
	for (auto it = parents.begin(); it != parents.end(); ++it)
	{
		emit model_->dataChanged( *it, *it, roles );
	}
}


void TreeExtension::onRowsAboutToBeRemoved( 
	const QModelIndex& parent, int first, int last )
{
	isolateRedundantIndices( parent, first, last,
		impl_->childModels_, impl_->redundantChildModels_ );
}


void TreeExtension::onRowsRemoved(
	const QModelIndex & parent, int first, int last )
{
	impl_->redundantChildModels_.clear();
}


/// Move to previous index
void TreeExtension::moveUp()
{
	int prevRow = impl_->currentIndex_.row() - 1;

	if (0 <= prevRow)
	{
		QModelIndex prevIndex = impl_->currentIndex_.sibling( prevRow, 0 );
		int prevIndexsBottomRow = 0;

		do {
			// Previous item's bottom row
			prevIndexsBottomRow = model_->rowCount( prevIndex ) - 1;

			impl_->currentIndex_ = prevIndex;

			if (model_->hasChildren( impl_->currentIndex_ ))
			{
				// Keep search in child tree if the bottom item has child tree and expanded
				prevIndexsBottomRow = model_->rowCount( impl_->currentIndex_ ) - 1;

				prevIndex = impl_->currentIndex_.child( prevIndexsBottomRow, 0 );
			}
		} while (model_->hasChildren( impl_->currentIndex_ ) && impl_->expanded( impl_->currentIndex_ ));

		emit currentIndexChanged();
	}
	else
	{
		// We are the first child, move up to the parent
		QModelIndex parent = impl_->currentIndex_.parent();

		if (parent.isValid())
		{
			// Update the current index if the parent is valid
			impl_->currentIndex_ = parent;
			emit currentIndexChanged();
		}
	}
}


/// Move to next index
void TreeExtension::moveDown()
{
	if (impl_->expanded( impl_->currentIndex_ ))
	{
		// Move to the first child item when the current item is expanded
		impl_->currentIndex_ = impl_->currentIndex_.child( 0, 0 );
		emit currentIndexChanged();
	}
	else
	{
		QModelIndex parent = impl_->currentIndex_.parent();

		int nextRow = impl_->currentIndex_.row() + 1;
		while (parent.isValid())
		{
			if (nextRow < model_->rowCount( parent ))
			{
				// Update the current index if the next item is available
				impl_->currentIndex_ = parent.child( nextRow, 0 );
				emit currentIndexChanged();
				break;
			}
			else
			{
				// Reached the bottom, keep searching the parent
				nextRow = parent.row() + 1;
				parent = parent.parent();
			}
		}
	}
}


/// Collapse the current item if it is collapsible or move to the parent
void TreeExtension::moveLeft()
{
	// Move up to the parent if there are no children or not expanded
	if (!model_->hasChildren( impl_->currentIndex_ ) || !impl_->expanded( impl_->currentIndex_ ))
	{
		// Move up to the parent
		QModelIndex parent = impl_->currentIndex_.parent();

		if (parent.isValid())
		{
			// Update the current index if the parent is valid
			impl_->currentIndex_ = parent;
			emit currentIndexChanged();
		}
	}
	else
	{
		// Collapse the current item
		int expandedRole = -1;
		this->encodeRole( ExpandedRole::roleId_, expandedRole );

		setData( impl_->currentIndex_, QVariant( false ), expandedRole );
	}
}


/// Expand the current item if it is expandable or move to the first child
void TreeExtension::moveRight()
{
	// Make sure the current item has children
	if (model_->hasChildren( impl_->currentIndex_ ) )
	{
		if (impl_->expanded( impl_->currentIndex_ ))
		{
			// Select the first child if the current item is expanded
			impl_->currentIndex_ = impl_->currentIndex_.child( 0, 0 );
			emit currentIndexChanged();
		}
		else
		{
			// Expand the current item
			int expandedRole = -1;
			this->encodeRole( ExpandedRole::roleId_, expandedRole );

			setData( impl_->currentIndex_, QVariant( true ), expandedRole );
		}
	}
}


QVariant TreeExtension::getCurrentIndex() const
{
	return QVariant::fromValue( impl_->currentIndex_ );
}


void TreeExtension::setCurrentIndex( const QVariant& index )
{
	QModelIndex idx = index.toModelIndex();
	impl_->currentIndex_ = idx;

	emit currentIndexChanged();
}