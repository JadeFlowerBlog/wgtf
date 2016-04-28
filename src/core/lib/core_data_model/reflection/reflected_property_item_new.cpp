#include "reflected_property_item_new.hpp"

#include "class_definition_model_new.hpp"
#include "reflected_object_item_new.hpp"
#include "reflected_enum_model_new.hpp"

//#include "core_data_model/generic_tree_model.hpp"
#include "core_data_model/i_item_role.hpp"
#include "core_reflection/interfaces/i_base_property.hpp"
#include "core_reflection/interfaces/i_reflection_controller.hpp"
#include "core_reflection/metadata/meta_impl.hpp"
#include "core_reflection/metadata/meta_utilities.hpp"
//#include "core_logging/logging.hpp"

#include "core_string_utils/string_utils.hpp"

#include <memory>
#include <codecvt>
#include <limits>

namespace
{
	struct MaxMinValuePair
	{
		MaxMinValuePair( const Variant & min, const Variant & max )
			: minValue_( min )
			, maxValue_( max )
		{
		}

		Variant minValue_;
		Variant maxValue_;
	};

	typedef std::unordered_map<const TypeId, MaxMinValuePair> MaxMinValuePairMap;

	MaxMinValuePair getValuePair(const TypeId& tid)
	{
			static const TypeId int8Type = TypeId::getType<int8_t>();
			static const TypeId int16Type = TypeId::getType<int16_t>();
			static const TypeId int32Type = TypeId::getType<int32_t>();
			static const TypeId int64Type = TypeId::getType<int64_t>();
			static const TypeId uint8Type = TypeId::getType<uint8_t>();
			static const TypeId uint16Type = TypeId::getType<uint16_t>();
			static const TypeId uint32Type = TypeId::getType<uint32_t>();
			static const TypeId uint64Type = TypeId::getType<uint64_t>();
			static const TypeId longType = TypeId::getType<long>();
			static const TypeId ulongType = TypeId::getType<unsigned long>();
			static const TypeId floatType = TypeId::getType<float>();
			static const TypeId doubleType = TypeId::getType<double>();

			if (int8Type == tid)
				return MaxMinValuePair( std::numeric_limits<int8_t>::lowest(), std::numeric_limits<int8_t>::max() );

			if (int16Type == tid)
				return MaxMinValuePair( std::numeric_limits<int16_t>::lowest(), std::numeric_limits<int16_t>::max() );

			if (int32Type == tid)
				return MaxMinValuePair( std::numeric_limits<int32_t>::lowest(), std::numeric_limits<int32_t>::max() );

			if (int64Type == tid)
				return MaxMinValuePair( std::numeric_limits<int64_t>::lowest(), std::numeric_limits<int64_t>::max() );


			if (uint8Type == tid)
				return MaxMinValuePair( std::numeric_limits<uint8_t>::min(), std::numeric_limits<uint8_t>::max() );

			if (uint16Type == tid)
				return MaxMinValuePair( std::numeric_limits<uint16_t>::min(), std::numeric_limits<uint16_t>::max() );

			if (uint32Type == tid)
				return MaxMinValuePair( std::numeric_limits<uint8_t>::min(), std::numeric_limits<uint32_t>::max() );

			if (uint64Type == tid)
				return MaxMinValuePair( std::numeric_limits<uint16_t>::min(), std::numeric_limits<uint64_t>::max() );


			if (longType == tid)
				return MaxMinValuePair( std::numeric_limits<long>::lowest(), std::numeric_limits<long>::max() );

			if (ulongType == tid)
				return MaxMinValuePair( std::numeric_limits<unsigned long>::min(), std::numeric_limits<unsigned long>::max() );

			if (floatType == tid)
				return MaxMinValuePair( std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max() );

			if (doubleType == tid)
				return MaxMinValuePair( std::numeric_limits<double>::lowest(), std::numeric_limits<double>::max() );

		return MaxMinValuePair(Variant(), Variant());
	}

	Variant getMaxValue( const TypeId & typeId )
	{
		return getValuePair(typeId).maxValue_;
	}

	Variant getMinValue( const TypeId & typeId )
	{
		return getValuePair(typeId).minValue_;
	}
}


class ReflectedPropertyItemNew::Implementation
{
public:
	Implementation( IComponentContext & contextManager );
	IComponentContext & contextManager_;
	std::string displayName_;
	std::vector< std::unique_ptr< ReflectedTreeItemNew > > children_;
};


ReflectedPropertyItemNew::Implementation::Implementation(
	IComponentContext & contextManager )
	: contextManager_( contextManager )
{
}


ReflectedPropertyItemNew::ReflectedPropertyItemNew( IComponentContext & contextManager,
	const IBasePropertyPtr & property,
	ReflectedTreeItemNew * parent,
	const std::string & inPlacePath )
	: ReflectedTreeItemNew( contextManager,
		parent,
		std::string( inPlacePath ) + property->getName() )
	, impl_( new Implementation( contextManager ) )
{
	// Must have a parent
	assert( parent != nullptr );
	assert( !path_.empty() );

	const MetaDisplayNameObj * displayName = nullptr;
	auto pDefinitionManager = this->getDefinitionManager();
	if (pDefinitionManager != nullptr)
	{
		displayName = findFirstMetaData< MetaDisplayNameObj >( *property, *pDefinitionManager );
	}

	if (displayName == nullptr)
	{
		impl_->displayName_ = property->getName();
		return;
	}
	std::wstring_convert< Utf16to8Facet > conversion( Utf16to8Facet::create() );
	impl_->displayName_ = conversion.to_bytes( displayName->getDisplayName() );
}


ReflectedPropertyItemNew::ReflectedPropertyItemNew( IComponentContext & contextManager,
	const std::string & propertyName,
	std::string displayName,
	ReflectedTreeItemNew * parent )
	: ReflectedTreeItemNew( contextManager,
		parent,
		parent ? parent->getPath() + propertyName : "" )
	, impl_( new Implementation( contextManager ) )
{
	impl_->displayName_ = std::move( displayName );

	// Must have a parent
	assert( parent != nullptr );
	assert( !path_.empty() );
}


ReflectedPropertyItemNew::~ReflectedPropertyItemNew()
{
}


const ObjectHandle & ReflectedPropertyItemNew::getRootObject() const /* override */
{
	return parent_->getRootObject();
}


const ObjectHandle & ReflectedPropertyItemNew::getObject() const /* override */
{
	return parent_->getObject();
}


//const char * ReflectedPropertyItemNew::getDisplayText( int column ) const
//{
//	switch (column)
//	{
//	case 0:
//		return impl_->displayName_.c_str();

//	case 1:
//		return "Reflected Property";

//	default:
//		assert( false );
//		return "";
//	}
//}


Variant ReflectedPropertyItemNew::getData( int column, size_t roleId ) const
{
	auto pDefinitionManager = this->getDefinitionManager();
	if (pDefinitionManager == nullptr)
	{
		return Variant();
	}

	auto obj = getObject();
	auto propertyAccessor = obj.getDefinition( *pDefinitionManager )->bindProperty(
		path_.c_str(), obj );

	if (roleId == IndexPathRole::roleId_)
	{
		return this->getPath();
	}
	else if (roleId == ObjectRole::roleId_)
	{
		return getObject();
	}
	else if (roleId == RootObjectRole::roleId_)
	{
		return getRootObject();
	}
	else if (roleId == ValueTypeRole::roleId_)
	{
		return propertyAccessor.getType().getName();
	}
	else if (roleId == IsEnumRole::roleId_)
	{
		return findFirstMetaData< MetaEnumObj >( propertyAccessor, *pDefinitionManager ) != nullptr;
	}
	else if (roleId == IsThumbnailRole::roleId_)
	{
		return findFirstMetaData< MetaThumbnailObj >( propertyAccessor, *pDefinitionManager ) != nullptr;
	}
	else if (roleId == IsSliderRole::roleId_)
	{
		return findFirstMetaData< MetaSliderObj >( propertyAccessor, *pDefinitionManager ) != nullptr;
	}
	else if (roleId == IsColorRole::roleId_)
	{
		return findFirstMetaData< MetaColorObj >( propertyAccessor, *pDefinitionManager ) != nullptr;
	}
	else if (roleId == IsUrlRole::roleId_)
	{
		return findFirstMetaData< MetaUrlObj >( propertyAccessor, *pDefinitionManager ) != nullptr;
	}
	else if (roleId == ValueRole::roleId_)
	{
		if (!propertyAccessor.canGetValue())
		{
			return Variant();
		}
		return propertyAccessor.getValue();
	}
	else if (roleId == MinValueRole::roleId_)
	{
		TypeId typeId = propertyAccessor.getType();
		Variant variant = getMinValue( typeId );
		auto minMaxObj =
			findFirstMetaData< MetaMinMaxObj >( propertyAccessor, *pDefinitionManager );
		if( minMaxObj != nullptr)
		{
			const float & value = minMaxObj->getMin();
			float minValue = .0f;
			bool isOk = variant.tryCast( minValue );
			assert( isOk );
			if (!isOk)
			{
				return variant;
			}
			float diff = minValue - value;
			float epsilon = std::numeric_limits<float>::epsilon();
			if (diff > epsilon )
			{
				NGT_ERROR_MSG("Property %s: MetaMinMaxObj min value exceeded limits.\n", path_.c_str());
				return variant;
			}
			return value;
		}
		else
		{
			return variant;
		}
	}
	else if (roleId == MaxValueRole::roleId_)
	{
		TypeId typeId = propertyAccessor.getType();
		Variant variant = getMaxValue( typeId );
		auto minMaxObj =
			findFirstMetaData< MetaMinMaxObj >( propertyAccessor, *pDefinitionManager );
		if( minMaxObj != nullptr)
		{
			const float & value = minMaxObj->getMax();
			float maxValue = .0f;
			bool isOk = variant.tryCast( maxValue );
			assert( isOk );
			if (!isOk)
			{
				return variant;
			}
			float diff = value - maxValue;
			float epsilon = std::numeric_limits<float>::epsilon();
			if (diff > epsilon)
			{
				NGT_ERROR_MSG("Property %s: MetaMinMaxObj max value exceeded limits.\n", path_.c_str());
				return variant;
			}
			return value;
		}
		else
		{
			return variant;
		}
	}
	else if (roleId == StepSizeRole::roleId_)
	{
		TypeId typeId = propertyAccessor.getType();
		auto stepSize = findFirstMetaData< MetaStepSizeObj >( propertyAccessor, *pDefinitionManager );
		if ( stepSize != nullptr )
		{
			return stepSize->getStepSize();
		}
		else
		{
			return MetaStepSizeObj::DefaultStepSize;
		}
	}
	else if (roleId == DecimalsRole::roleId_)
	{
		TypeId typeId = propertyAccessor.getType();
		auto decimals = findFirstMetaData< MetaDecimalsObj >( propertyAccessor, *pDefinitionManager );
		if ( decimals != nullptr )
		{
			return decimals->getDecimals();
		}
		else
		{
			return MetaDecimalsObj::DefaultDecimals;
		}
	}
	else if (roleId == EnumModelRole::roleId_)
	{
		auto enumObj = findFirstMetaData< MetaEnumObj >( propertyAccessor, *pDefinitionManager );
		if (enumObj)
		{
			if (getObject().isValid() == false )
			{
				return Variant();
			}
			auto enumModel = std::unique_ptr< AbstractListModel >( 
				new ReflectedEnumModelNew( propertyAccessor, enumObj ) );
			return ObjectHandle( std::move( enumModel ) );
		}
	}
	else if (roleId == DefinitionRole::roleId_)
	{
		if (!propertyAccessor.canGetValue())
		{
			return Variant();
		}

		auto variant = propertyAccessor.getValue();
		ObjectHandle provider;
		variant.tryCast( provider );
		provider = reflectedRoot( provider, *pDefinitionManager );
		auto definition = const_cast< IClassDefinition * >(
			provider.isValid() ? provider.getDefinition( *pDefinitionManager ) : nullptr );
		return ObjectHandle( definition );
	}
	else if (roleId == DefinitionModelRole::roleId_)
	{
		TypeId typeId = propertyAccessor.getType();
		if (typeId.isPointer())
		{
			auto definition = pDefinitionManager->getDefinition( typeId.removePointer().getName() );
			if(definition != nullptr)
			{
				auto definitionModel = std::unique_ptr< AbstractListModel >(
					new ClassDefinitionModelNew( definition, *pDefinitionManager ) );
				return ObjectHandle( std::move( definitionModel ) );
			}
		}
	}
	else if (roleId == UrlIsAssetBrowserRole::roleId_)
	{
		bool isAssetBrowserDlg = false;
		auto urlObj =
			findFirstMetaData< MetaUrlObj >( propertyAccessor, *pDefinitionManager );
		if( urlObj != nullptr)
		{
			isAssetBrowserDlg = urlObj->isAssetBrowserDialog();
		}
		return isAssetBrowserDlg;
	}
	else if (roleId == UrlDialogTitleRole::roleId_)
	{
		const char * title;
		auto urlObj =
			findFirstMetaData< MetaUrlObj >( propertyAccessor, *pDefinitionManager );
		if( urlObj != nullptr)
		{
			title = urlObj->getDialogTitle();
		}
		return title;
	}
	else if (roleId == UrlDialogDefaultFolderRole::roleId_)
	{
		const char * folder;
		auto urlObj =
			findFirstMetaData< MetaUrlObj >( propertyAccessor, *pDefinitionManager );
		if( urlObj != nullptr)
		{
			folder = urlObj->getDialogDefaultFolder();
		}
		return folder;
	}
	else if (roleId == UrlDialogNameFiltersRole::roleId_)
	{
		const char * nameFilters;
		auto urlObj =
			findFirstMetaData< MetaUrlObj >( propertyAccessor, *pDefinitionManager );
		if( urlObj != nullptr)
		{
			nameFilters = urlObj->getDialogNameFilters();
		}
		return nameFilters;
	}
	else if (roleId == UrlDialogSelectedNameFilterRole::roleId_)
	{
		const char * selectedFilter;
		auto urlObj =
			findFirstMetaData< MetaUrlObj >( propertyAccessor, *pDefinitionManager );
		if( urlObj != nullptr)
		{
			selectedFilter = urlObj->getDialogSelectedNameFilter();
		}
		return selectedFilter;
	}
	else if (roleId == UrlDialogModalityRole::roleId_)
	{
		int modality = 1;
		auto urlObj =
			findFirstMetaData< MetaUrlObj >( propertyAccessor, *pDefinitionManager );
		if( urlObj != nullptr)
		{
			const int & value = urlObj->getDialogModality();
			if (value >= 0 && value <= 2)
			{
				modality = value;
			}
		}
		return modality;
	}
	else if ( roleId == IsReadOnlyRole::roleId_ )
	{
		TypeId typeId = propertyAccessor.getType();
		auto readonly =
			findFirstMetaData< MetaReadOnlyObj >( propertyAccessor, *pDefinitionManager );
		if ( readonly != nullptr )
		{
			return true;
		}
		return false;
	}
	return Variant();
}


bool ReflectedPropertyItemNew::setData( int column, size_t roleId, const Variant & data )
{
	auto controller = getController();
	if (controller == nullptr)
	{
		return false;
	}
	auto pDefinitionManager = this->getDefinitionManager();
	if (pDefinitionManager == nullptr)
	{
		return 0;
	}

	auto obj = getObject();
	auto propertyAccessor = obj.getDefinition( *pDefinitionManager )->bindProperty(
		path_.c_str(), obj );

	if (roleId == ValueRole::roleId_)
	{
		controller->setValue( propertyAccessor, data );
		return true;
	}
	else if (roleId == DefinitionRole::roleId_)
	{
		TypeId typeId = propertyAccessor.getType();
		if (!typeId.isPointer())
		{
			return false;
		}

		auto baseDefinition = pDefinitionManager->getDefinition(
			typeId.removePointer().getName() );
		if(baseDefinition == nullptr)
		{
			return false;
		}

		ObjectHandle provider;
		if (!data.tryCast< ObjectHandle >( provider ))
		{
			return false;
		}

		auto valueDefinition = provider.getBase< IClassDefinition >();
		if (valueDefinition == nullptr)
		{
			return false;
		}

		ObjectHandle value;
		value = valueDefinition->create();
		controller->setValue( propertyAccessor, value );
		return true;
	}
	return false;
}


ReflectedTreeItemNew * ReflectedPropertyItemNew::getChild( size_t index ) const
{
	if (impl_->children_.size() <= index )
	{
		impl_->children_.reserve( index + 1 );
		while (impl_->children_.size() <= index)
		{
			impl_->children_.emplace_back( nullptr );
		}
	}

	auto child = impl_->children_[index].get();
	if (child != nullptr)
	{
		return child;
	}

	auto obj = this->getObject();
	auto pDefinitionManager = this->getDefinitionManager();
	if (pDefinitionManager == nullptr)
	{
		return nullptr;
	}
	auto propertyAccessor = obj.getDefinition( *pDefinitionManager )->bindProperty(
		path_.c_str(), obj );

	if (!propertyAccessor.canGetValue())
	{
		return nullptr;
	}

	Collection collection;
	const bool isCollection = propertyAccessor.getValue().tryCast( collection );
	if (isCollection)
	{
		size_t i = 0;
		auto it = collection.begin();

		for (; i < index && it != collection.end(); ++it)
		{
			++i;
		}

		if (it == collection.end())
		{
			return nullptr;
		}

		{
			// FIXME NGT-1603: Change to actually get the proper key type

			// Attempt to use an index into the collection
			// Defaults to i
			size_t indexKey = i;
			const bool isIndex = it.key().tryCast( indexKey );

			// Default to using an index
			std::string propertyName =
					"[" + std::to_string( static_cast< int >( indexKey ) ) + "]";
			std::string displayName = propertyName;

			// If the item isn't an index
			if (!isIndex)
			{
				// Try to cast the key to a string
				const bool isString = it.key().tryCast( displayName );
				if (isString)
				{
					// Strings must be quoted to work with TextStream
					propertyName = "[\"" + displayName + "\"]";
				}
			}

			child = new ReflectedPropertyItemNew( impl_->contextManager_,
				propertyName,
				std::move( displayName ),
				const_cast< ReflectedPropertyItemNew * >( this ) );
		}
		impl_->children_[index] = std::unique_ptr< ReflectedTreeItemNew >( child );
		return child;
	}


	auto value = propertyAccessor.getValue();
	ObjectHandle baseProvider;
	value.tryCast( baseProvider );
	if (!baseProvider.isValid())
	{
		return nullptr;
	}
	baseProvider = reflectedRoot( baseProvider, *pDefinitionManager );
	child = new ReflectedObjectItemNew( impl_->contextManager_,
		baseProvider ,
		const_cast< ReflectedPropertyItemNew * >( this ) );
	child->hidden( true );
	impl_->children_[index] = std::unique_ptr< ReflectedTreeItemNew >( child );
	return child;
}


int ReflectedPropertyItemNew::rowCount() const
{
	auto pDefinitionManager = this->getDefinitionManager();
	if (pDefinitionManager == nullptr)
	{
		return 0;
	}
	auto obj = getObject();
	auto propertyAccessor = obj.getDefinition( *pDefinitionManager )->bindProperty(
		path_.c_str(), obj );

	if (!propertyAccessor.canGetValue())
	{
		return 0;
	}

	Collection collection;
	const Variant & value = propertyAccessor.getValue();
	bool isCollection = value.tryCast( collection );
	if (isCollection)
	{
		return static_cast< int >( collection.size() );
	}

	ObjectHandle handle;
	bool isObjectHandle = value.tryCast( handle );
	if(isObjectHandle)
	{
		handle = reflectedRoot( handle, *pDefinitionManager );
		auto def = handle.getDefinition( *pDefinitionManager );
		if(def != nullptr)
		{
			return 1;
		}
	}

	return 0;
}


bool ReflectedPropertyItemNew::preSetValue( const PropertyAccessor & accessor,
	const Variant & value )
{
	auto pDefinitionManager = this->getDefinitionManager();
	if (pDefinitionManager == nullptr)
	{
		return false;
	}

	auto obj = getObject();
	auto otherObj = accessor.getObject();
	auto otherPath = accessor.getFullPath();

	if (obj == otherObj && path_ == otherPath)
	{
		TypeId typeId = accessor.getType();
		bool isReflectedObject = 
			typeId.isPointer() &&
			pDefinitionManager->getDefinition( typeId.removePointer().getName() ) != nullptr;
		if(isReflectedObject)
		{
			const IClassDefinition * definition = nullptr;
			ObjectHandle handle;
			if (value.tryCast( handle ))
			{
				definition = handle.getDefinition( *pDefinitionManager );
			}
			//getModel()->signalPreItemDataChanged( this, 1, DefinitionRole::roleId_,
			//	ObjectHandle( definition ) );
			return true;
		}

		//getModel()->signalPreItemDataChanged( this, 1, ValueRole::roleId_,
		//	value );
		return true;
	}

	for (auto it = impl_->children_.begin(); it != impl_->children_.end(); ++it)
	{
		if ((*it) == nullptr)
		{
			continue;
		}

		if ((*it)->preSetValue( accessor, value ))
		{
			return true;
		}
	}
	return false;
}


bool ReflectedPropertyItemNew::postSetValue( const PropertyAccessor & accessor,
	const Variant & value )
{
	auto pDefinitionManager = this->getDefinitionManager();
	if (pDefinitionManager == nullptr)
	{
		return false;
	}

	auto obj = getObject();
	auto otherObj = accessor.getObject();
	auto otherPath = accessor.getFullPath();

	if (obj == otherObj && path_ == otherPath)
	{
		TypeId typeId = accessor.getType();
		bool isReflectedObject = 
			typeId.isPointer() &&
			pDefinitionManager->getDefinition( typeId.removePointer().getName() ) != nullptr;
		if(isReflectedObject)
		{
			const IClassDefinition * definition = nullptr;
			ObjectHandle handle;
			if (value.tryCast( handle ))
			{
				definition = handle.getDefinition( *pDefinitionManager );
			}
			impl_->children_.clear();
			//getModel()->signalPostItemDataChanged( this, 1, DefinitionRole::roleId_,
			//	ObjectHandle( definition ) );
			return true;
		}

		//getModel()->signalPostItemDataChanged( this, 1, ValueRole::roleId_,
		//	value );
		return true;
	}

	for (auto it = impl_->children_.begin(); it != impl_->children_.end(); ++it)
	{
		if ((*it) == nullptr)
		{
			continue;
		}

		if ((*it)->postSetValue( accessor, value ))
		{
			return true;
		}
	}
	return false;
}
