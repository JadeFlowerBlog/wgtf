#ifndef I_INTERFACE_HPP
#define I_INTERFACE_HPP

#include <vector>
#include "interface_helpers.hpp"
#include "core_variant/type_id.hpp"
#include <type_traits>

class IInterface;

namespace Context
{
	extern void * queryInterface( const TypeId & );
	extern void queryInterface( const TypeId &, std::vector< void * > & );
}

//==============================================================================
class IInterface
{
public:
	virtual ~IInterface() {}
	virtual void * queryInterface( const TypeId & id ) = 0;
};


//==============================================================================
//Used to as null class type
struct EmptyType { };

//==============================================================================
// Change this to support as many classes as we need to inherit, until we have
// support for variadic templates
//==============================================================================
template<	typename T1,
			typename T2 = EmptyType,
			typename T3 = EmptyType,
			typename T4 = EmptyType,
			typename T5 = EmptyType>
class Implements
	: public T1
	, public virtual Implements< T2, T3, T4, T5 >
{
public:
	void * queryInterface( const TypeId & id )
	{
		static const TypeId selfType = TypeId::getType< T1 >();
		auto pT1 = static_cast< T1 * >( this );
		if (selfType == id)
		{
			return pT1;
		}
		void * t1Result = queryInterface( pT1, id );
		if (t1Result)
		{
			return t1Result;
		}
		return Implements< T2, T3, T4, T5 >::queryInterface( id );
	}

	template <class T>
	class _detector
	{
		template <class U, void* (U::*) (const TypeId& id)> struct _checker;
		
		template <class U> static int check(_checker<U, &U::queryInterface>*);
		static char check(...);
		
	public:
		static const bool _detected = (sizeof(check(0)) == sizeof(int));
	};

private:
	template< typename T >
	static void * queryInterface( T * pThis, const TypeId & id, typename std::enable_if<_detector<T>::_detected>::type* )
	{
		return T::queryInterface( pThis, id );
	}
	
	static void * queryInterface( ... )
	{
		return nullptr;
	}
};

//==============================================================================
// Handle the empty type
//==============================================================================
template<>
class Implements< EmptyType >
{
public:
	void * queryInterface( const TypeId & /*id*/ )
	{
		return nullptr;
	}
};

struct query_interface
{
	template<class T>
	static typename std::enable_if<std::is_base_of<Implements<EmptyType>, T>::value>::type*
		execute( T* pImpl_, const TypeId& id )
	{
		return pImpl_->queryInterface(id);
	}

	template<class T>
	static typename std::enable_if<!std::is_base_of<Implements<EmptyType>, T>::value>::type*
		execute(T* pImpl_, const TypeId& id)
	{
		static const TypeId selfType = TypeId::getType< T >();
		if ( selfType == id )
		{
			return pImpl_;
		}
		return nullptr;
	}
};

template< class T >
class InterfaceHolder
	: public IInterface
{
public:
	InterfaceHolder( T * pImpl, bool owns )
		: pImpl_( pImpl )
		, owns_( owns )
	{
	}

	~InterfaceHolder()
	{
		if(owns_)
		{
			delete pImpl_;
		}
	}

	void * queryInterface( const TypeId & id ) override
	{
		return query_interface::execute(pImpl_, id );
	}

private:
	T * pImpl_;
	bool owns_;
};

namespace Context
{

//==============================================================================
template< class T >
T * queryInterface()
{
	return reinterpret_cast< T * >( Context::queryInterface(
		typeid( T ).name() ) );
}

template< class T >
void queryInterface( std::vector< T * > & o_Impls )
{
	Context::queryInterface(
		typeid( T ).name(),
		reinterpret_cast< std::vector< void * > & >( o_Impls ) );
}

} //namespace Context

#endif //I_INTERFACE_HPP