#ifndef OBJECT_HANDLE_HPP
#define OBJECT_HANDLE_HPP

#define DEPRECATE_OBJECT_HANDLE 0
#if DEPRECATE_OBJECT_HANDLE
#define DEPRECATE_OBJECT_HANDLE_MSG( format, ... ) {				\
	char buffer[1024];												\
	sprintf_s( buffer, 1024, format, ## __VA_ARGS__ );				\
	assert( false && buffer );										\
	}
#else
#define DEPRECATE_OBJECT_HANDLE_MSG( format, ... ) {				\
	NGT_TRACE_MSG( format, ## __VA_ARGS__ );						\
	}
#endif

/*
An ObjectHandle contains a data type and its associated ClassDefintion.
ObjectHandles store reflection data at runtime and are flexible enough to store
most types, including references, pointers and structures.
Use an ObjectHandle when a model or property needs to be exposed to QML.
Also use when working with reflection containers or reflected objects.
Details: https://confluence.wargaming.net/display/NGT/NGT+Reflection+System
*/

#include "object_handle_storage.hpp"
#include "core_logging/logging.hpp"
#include "core_variant/type_id.hpp"
#include "interfaces/i_class_definition_details.hpp"
#include "interfaces/i_class_definition.hpp"
#include <cassert>
#include <memory>
#include <map>
#include <unordered_map>

template<typename T> class ObjectHandleT;
class ReflectedPolyStruct;

//==============================================================================
class ObjectHandle
{
public:
	static ObjectHandle getHandle( ReflectedPolyStruct & value );

	ObjectHandle();
	ObjectHandle( const ObjectHandle & other );
	ObjectHandle( ObjectHandle && other );
	ObjectHandle( const std::nullptr_t & );

	//--------------------------------------------------------------------------
	template< typename T >
	ObjectHandle( const ObjectHandleT< T > & other );

	//--------------------------------------------------------------------------
	template< typename T >
	ObjectHandle(
		const T & value,
		const IClassDefinition * definition = nullptr )
		: storage_(
		new ObjectHandleStorage< T >(
		const_cast< T & >( value ), definition ) )
	{
	}

	//--------------------------------------------------------------------------
	template< typename T >
	ObjectHandle(
		std::unique_ptr<T> && value,
		const IClassDefinition * definition = nullptr)
		: storage_(
		new ObjectHandleStorage< std::unique_ptr< T > >(
		std::move(value), definition ) )
	{
	}

	//--------------------------------------------------------------------------
	template< typename T >
	static ObjectHandle makeStorageBackedProvider(
		const T & value,
		const IClassDefinition * definition = nullptr )
	{
		ObjectHandle provider;
		provider.storage_.reset(
			new ObjectHandleStorageCopy< T >(
			const_cast< T & >( value ), definition ) );
		return provider;
	}

	//--------------------------------------------------------------------------
	template< class T >
	T * reflectedCast( const IDefinitionManager & definitionManager ) const
	{
		return reinterpret_cast< T * >( reflectedCast( TypeId::getType< T >(), definitionManager ) );
	}

	//--------------------------------------------------------------------------
	template< class T >
	T * getBase() const
	{
		if(storage_ == nullptr)
		{
			return nullptr;
		}
		static const TypeId s_Type = TypeId::getType< T >();
		if (s_Type != storage_->type())
		{
			try
			{
				storage_->throwBase();
			}
			catch (T* value)
			{
				DEPRECATE_OBJECT_HANDLE_MSG( "DEPRECATED OBJECTHANDLE: Type '%s' stored in ObjectHandle does not match type explicitly queried type '%s'\n", storage_->type().getName(), s_Type.getName() );
				return value;
			}
			catch(...)
			{
			}
			return nullptr;
		}
		return static_cast< T * >( storage_->data() );
	}

	void * data() const;
	TypeId type() const;
	bool isValid() const;

	const IClassDefinition * getDefinition( const IDefinitionManager & definitionManager ) const;
	bool getId( RefObjectId & o_Id ) const;
	void throwBase() const;
	bool operator ==( const ObjectHandle & other ) const;
	bool operator !=( const ObjectHandle & other ) const;
	ObjectHandle & operator=( const std::nullptr_t & );
	ObjectHandle & operator=( const ObjectHandle & other );
	ObjectHandle & operator=( ObjectHandle && other );
	bool operator<( const ObjectHandle & other ) const;

	//--------------------------------------------------------------------------
	template< typename T >
	ObjectHandle & operator=( const T & value )
	{
		static_assert(!std::is_copy_constructible<T>::value,
			"Type is not copy constructable, try using std::move(value)");
		storage_ .reset( new ObjectHandleStorage< T >( const_cast< T & >( value ) ) );
		return *this;
	}

	//--------------------------------------------------------------------------
	template< typename T >
	ObjectHandle & operator=( std::unique_ptr< T >&& value )
	{
		storage_.reset(new ObjectHandleStorage< std::unique_ptr< T > >(std::move(value)));
		return *this;
	}

	//--------------------------------------------------------------------------
	void * reflectedCast( const TypeId & typeId, const IDefinitionManager & definitionManager ) const;

private:
	template< typename T >
	friend class ObjectHandleT;

	std::shared_ptr< IObjectHandleStorage > storage_;
};



//==============================================================================
template< typename T >
class ObjectHandleT
{
public:
	//--------------------------------------------------------------------------
	ObjectHandleT()
	: storage_( nullptr )
	{
	}


	//--------------------------------------------------------------------------
	template< typename T2 >
	ObjectHandleT( const ObjectHandleT< T2 > & other )
	: storage_( other.storage_ )
	{
	}

	//--------------------------------------------------------------------------
	ObjectHandleT( const ObjectHandle & other )
	: storage_( other.storage_ )
	{
	}


	//--------------------------------------------------------------------------
	ObjectHandleT( const std::nullptr_t & )
	: storage_( nullptr )
	{
	}


	struct NoValue {};

	template <typename T1, typename T2 = NoValue, typename _dummy = void >
	struct _all
	: std::conditional< T1::value, _all< T2 >, std::false_type >::type
	{
	};


	template<typename _dummy>
	struct _all< NoValue, NoValue, _dummy >
	: std::true_type
	{
	};

	template< typename T1, typename _dummy = void >
	struct _not
	: std::false_type
	{
	};


	template<typename _dummy>
	struct _not< std::false_type, _dummy >
	: std::true_type
	{
	};


	//--------------------------------------------------------------------------
	//Allow conversions between const and non-const types
	template< typename ConvertType >
	ObjectHandleT(
								const ObjectHandleT< ConvertType > & handle,
								typename std::enable_if<
								_all< std::is_base_of< ConvertType, T >,
								_not<
								_all<
								std::is_const< T >,
								_not< std::is_const< ConvertType > >
								>
								>
								>::value >::type * = nullptr  )
	: storage_( handle->storage_ )
	{
	}

	//--------------------------------------------------------------------------
	T * get() const
	{
		auto handle = ObjectHandle( *this );
		return handle.getBase< T >();
	}


	//--------------------------------------------------------------------------
	const IClassDefinition * getDefinition( IDefinitionManager & definitionManager ) const
	{
		auto handle = ObjectHandle( *this );
		return handle.getDefinition( definitionManager );
	}


	//--------------------------------------------------------------------------
	bool getId( RefObjectId & o_Id ) const
	{
		return storage_ != nullptr ? storage_->getId( o_Id ) : false;
	}


	//--------------------------------------------------------------------------
	T & operator*() const
	{
		auto pObject = get();
		assert( pObject != nullptr );
		return *pObject;
	}


	//--------------------------------------------------------------------------
	T * operator->() const
	{
		return get();
	}


	//--------------------------------------------------------------------------
	bool operator ==( const ObjectHandle & other ) const
	{
		if (storage_ == other.storage_)
		{
			return true;
		}

		if (storage_ == nullptr || other.storage_ == nullptr)
		{
			return false;
		}

		auto left = storage_->data();
		auto right = other.storage_->data();
		if (left == right)
		{
			if (storage_->type() == other.storage_->type())
			{
				return true;
			}
		}
		return false;
	}



	//--------------------------------------------------------------------------
	bool operator==( const void * p ) const
	{
		return get() == p;
	}


	//--------------------------------------------------------------------------
	bool operator!=( const void * p ) const
	{
		return !operator==( p );
	}


	//--------------------------------------------------------------------------
	ObjectHandleT & operator=( const std::nullptr_t & )
	{
		storage_ = nullptr;
		return *this;
	}


	//--------------------------------------------------------------------------
	ObjectHandleT & operator=( const ObjectHandle & other )
	{
		storage_ = other.storage_;
		return *this;
	}



	//--------------------------------------------------------------------------
	template< typename T2 >
	ObjectHandleT & operator=( const ObjectHandleT< T2 > & other )
	{
		storage_ = other.storage_;
		return *this;
	}


	//--------------------------------------------------------------------------
	bool operator<( const ObjectHandle & other ) const
	{
		if (storage_ == other.storage_)
		{
			return false;
		}

		if (storage_ == nullptr)
		{
			return true;
		}

		if (other.storage_ == nullptr)
		{
			return false;
		}

		auto left = storage_->data();
		auto right = other.storage_->data();
		if (left == right)
		{
			return storage_->type() < other.storage_->type();
		}
		return left < right;
	}

private:
	friend ObjectHandle;
	template< typename T2 >
	friend class ObjectHandleT;

	std::shared_ptr< IObjectHandleStorage > storage_;
};

template< typename T >
ObjectHandle::ObjectHandle( const ObjectHandleT< T > & other )
: storage_( other.storage_ )
{
}

template<typename T>
ObjectHandle upcast( const ObjectHandleT< T > & v )
{
	return ObjectHandle( v );
}

#endif //OBJECT_HANDLE_HPP
