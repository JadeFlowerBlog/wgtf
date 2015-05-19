#include "child_list_adapter.hpp"

ChildListAdapter::ChildListAdapter( const QModelIndex & parent )
	: parent_( parent )
{
	connect();
}

ChildListAdapter::~ChildListAdapter()
{
	disconnect();
}

QAbstractItemModel * ChildListAdapter::model() const
{
	return const_cast< QAbstractItemModel * >( parent_.model() );
}

QModelIndex ChildListAdapter::adaptedIndex(int row, int column, const QModelIndex &parent) const
{
	return model()->index( row, column, parent_ );
}

int ChildListAdapter::rowCount(const QModelIndex &parent) const
{
	return model()->rowCount( parent_ );
}

void ChildListAdapter::onParentDataChanged(const QModelIndex &topLeft, 
	const QModelIndex &bottomRight, const QVector<int> &roles)
{
	if (topLeft.parent() == parent_ &&
		bottomRight.parent() == parent_)
	{
		emit dataChanged( 
			createIndex(topLeft.row(), topLeft.column()), 
			createIndex(bottomRight.row(), bottomRight.column()), 
			roles );
	}
}

void ChildListAdapter::onParentLayoutAboutToBeChanged(const QList<QPersistentModelIndex> & parents, 
												 QAbstractItemModel::LayoutChangeHint hint)
{
	if (parents.empty() || parents.contains( parent_ ))
	{
		reset();
		emit layoutAboutToBeChanged();
	}
}

void ChildListAdapter::onParentLayoutChanged(const QList<QPersistentModelIndex> & parents, 
										QAbstractItemModel::LayoutChangeHint hint)
{
	if (parents.empty() || parents.contains( parent_ ))
	{
		emit layoutChanged();
	}
}

void ChildListAdapter::onParentRowsAboutToBeInserted(const QModelIndex & parent, int first, int last)
{
	if (parent != parent_)
	{
		return;
	}

	beginInsertRows( QModelIndex(), first, last );
}

void ChildListAdapter::onParentRowsInserted(const QModelIndex & parent, int first, int last)
{
	if (parent != parent_)
	{
		return;
	}

	reset();
	endInsertRows();
}

void ChildListAdapter::onParentRowsAboutToBeRemoved(const QModelIndex & parent, int first, int last)
{
	if (parent != parent_)
	{
		return;
	}

	removedParent_ = parent_;
	beginRemoveRows( QModelIndex(), first, last );
}

void ChildListAdapter::onParentRowsRemoved(const QModelIndex & parent, int first, int last)
{
	if (removedParent_ == QModelIndex())
	{
		return;
	}

	removedParent_ = QModelIndex();
	reset();
	endRemoveRows();
}

