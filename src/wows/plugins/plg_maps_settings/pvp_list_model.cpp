#include "pvp_list_model.hpp"
#include "core_data_model/i_item.hpp"
#include "core_data_model/i_item_role.hpp"
#include "core_variant/collection.hpp"
#include "core_serialization/resizing_memory_stream.hpp"


namespace
{
	class HeaderItem : public IItem
	{
	public:
		HeaderItem( PvpListModel & model, size_t index )
			: model_( model )
			, index_( index )
		{

		}

		const char * getDisplayText( int column ) const override
		{
			return nullptr;
		}

		ThumbnailData getThumbnail( int column ) const override
		{
			return nullptr;
		}

		Variant getData( int column, size_t roleId ) const override
		{
			auto & collection = model_.getSource();
			if (roleId == ValueTypeRole::roleId_)
			{
				return collection.valueType().getName();
			}
			else if (roleId == KeyTypeRole::roleId_)
			{
				return collection.keyType().getName();
			}
			
			auto it = collection.begin();
			for (size_t i = 0; i < index_ && it != collection.end(); ++i, ++it) {}
			if (it == collection.end())
			{
				return Variant();
			}

			if (roleId == IndexPathRole::roleId_)
			{
				ResizingMemoryStream dataStream;
				TextStream s(dataStream);
				Variant value = it.value();
				s << value;
				return dataStream.takeBuffer();
			}

			if (roleId == ValueRole::roleId_)
			{
				//return it.value();

				if (column == 1)
				{
					return "Map/Scenario";
				}
				else
				{
					const int tier = column - 1;
					if (tier <= 9)
					{
						const char digit = '0' + tier;
						return std::string( "t" ) + digit;
					}
					else
					{
						const char digit = '0' + (tier - 10);
						return std::string( "t1" ) + digit;
					}
				}
			}
			else if (roleId == KeyRole::roleId_)
			{
				return it.key();
			}

			return Variant();
		}

		bool setData( int column, size_t roleId, const Variant & data ) override
		{
			assert( false && "Not implemented" );
			return false;
		}

	private:
		PvpListModel & model_;
		size_t index_;
	};

	class CollectionItem : public IItem
	{
	public:
		CollectionItem( PvpListModel & model, size_t index )
			: model_( model )
			, index_( index )
		{

		}

		const char * getDisplayText( int column ) const override
		{
			return nullptr;
		}

		ThumbnailData getThumbnail( int column ) const override
		{
			return nullptr;
		}

		Variant getData( int column, size_t roleId ) const override
		{
			auto & collection = model_.getSource();
			if (roleId == ValueTypeRole::roleId_)
			{
				return collection.valueType().getName();
			}
			else if (roleId == KeyTypeRole::roleId_)
			{
				return collection.keyType().getName();
			}
			
			auto it = collection.begin();
			for (size_t i = 0; i < index_ && it != collection.end(); ++i, ++it) {}
			if (it == collection.end())
			{
				return Variant();
			}

			if (roleId == IndexPathRole::roleId_)
			{
				ResizingMemoryStream dataStream;
				TextStream s(dataStream);
				Variant value = it.value();
				s << value;
				return dataStream.takeBuffer();
			}

			if (roleId == ValueRole::roleId_)
			{
				//return it.value();

				if (column == 1)
				{
					return "(33) spaces/00_CO_ocean";
				}
				else
				{
					return 0;
				}
			}
			else if (roleId == KeyRole::roleId_)
			{
				return it.key();
			}

			return Variant();
		}

		bool setData( int column, size_t roleId, const Variant & data ) override
		{
			//if (roleId != ValueRole::roleId_)
			//{
			//	return false;
			//}

			//auto & collection = model_.getSource();
			//auto it = collection.begin();
			//for (size_t i = 0; i < index_ && it != collection.end(); ++i, ++it) {}
			//if (it == collection.end())
			//{
			//	return false;
			//}

			//it.setValue( data );
			//return true;
			assert( false && "Not implemented" );
			return false;
		}

	private:
		PvpListModel & model_;
		size_t index_;
	};
} // namespace


PvpListModel::PvpListModel( Collection & source )
{
	this->setSource( source );
}


PvpListModel::~PvpListModel()
{
}


IItem * PvpListModel::item( size_t index ) const
{
	if (items_.size() <= index)
	{
		items_.resize( index + 1 );
	}

	auto item = items_[index].get();
	if (item != nullptr)
	{
		return item;
	}

	if (index == 0)
	{
		item = new HeaderItem( *const_cast< PvpListModel * >( this ), index );
		items_[index] = std::unique_ptr< IItem >( item );
		return item;
	}

	item = new CollectionItem( *const_cast< PvpListModel * >( this ), index - 1 );
	items_[index] = std::unique_ptr< IItem >( item );
	return item;
}


size_t PvpListModel::index( const IItem* item ) const
{
	auto index = 0;
	auto it = items_.begin();
	for (; it != items_.end() && it->get() != item; ++index, ++it) {}
	assert( it != items_.end() );
	return index;
}


bool PvpListModel::empty() const
{
	return collection_.empty();
}


size_t PvpListModel::size() const
{
	return collection_.size() + 1;
}


int PvpListModel::columnCount() const
{
	return 11;
}
