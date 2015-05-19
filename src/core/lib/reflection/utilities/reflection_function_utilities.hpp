#ifndef REFLECTION_FUNCTION_UTILITIES_HPP
#define REFLECTION_FUNCTION_UTILITIES_HPP

#include "variant/type_id.hpp"
#include "reflection/reflected_property.hpp"
#include <type_traits>

template <typename SelfType>
struct FunctionPropertyHelper
{
	template <typename DataType>
	static BaseProperty * getBaseProperty(
		const char * name, DataType SelfType::* memberPointer)
	{
		return new ReflectedProperty< DataType, SelfType >(
			name, memberPointer, 
			TypeId::getType< DataType >() );
	}


	template <typename DataType >
	static BaseProperty * getBaseProperty(
		const char * name,
		DataType (SelfType::*getterFunc)() const,
		typename FunctionPropertySet< typename std::decay< DataType >::type, SelfType >::SetterFunc setterFunc = NULL )
	{
		return new FunctionProperty< typename std::decay< DataType >::type, SelfType, !std::is_reference< DataType >::value, false >(
			name, getterFunc, setterFunc,
			TypeId::getType< typename std::decay< DataType >::type >() );
	}

	template <typename DataType>
	static BaseProperty * getBaseProperty(
		const char * name,
		void (SelfType::*getterFunc)( DataType * ) const,
		typename FunctionPropertySet< DataType, SelfType >::SetterFunc setterFunc = NULL )
	{
		return new FunctionProperty< DataType, SelfType, true, true >(
			name, getterFunc, setterFunc,
			TypeId::getType< DataType >() );
	}

	template <typename DataType>
	static BaseProperty * getBaseProperty(
		const char * name,
		size_t ( SelfType::*getSizeFunc )() const,
		DataType & ( SelfType::*getValueFunc )( size_t ) const )
	{
		return new FunctionCollectionProperty< size_t, DataType, SelfType >(
			name, getSizeFunc, nullptr, getValueFunc, nullptr );
	}


	template <typename DataType, typename KeyType >
	static BaseProperty * getBaseProperty(
		const char * name,
		size_t ( SelfType::*getSizeFunc )() const,
		DataType & ( SelfType::*getValueFunc )( size_t ) const,
		size_t ( SelfType::*setKeyFunc )( const KeyType & ) = nullptr,
		KeyType & ( SelfType::*getKeyFunc )( size_t ) const = nullptr )
	{
		return new FunctionCollectionProperty< KeyType, DataType, SelfType >(
			name, getSizeFunc, getKeyFunc, getValueFunc, setKeyFunc );
	}
};

#endif //REFLECTION_FUNCTION_UTILITIES_HPP