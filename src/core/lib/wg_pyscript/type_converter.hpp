#pragma once
#ifndef TYPE_CONVERTER_HPP
#define TYPE_CONVERTER_HPP


#include <cstdint>
#include <map>
#include <string>
#include <vector>


/// This type is an unsigned integer.
typedef unsigned int	uint;

/// This type is an integer with a size of 8 bits.
typedef int8_t				int8;
/// This type is an unsigned integer with a size of 8 bits.
typedef uint8_t				uint8;

/// This type is an integer with a size of 16 bits.
typedef int16_t				int16;
/// This type is an unsigned integer with a size of 16 bits.
typedef uint16_t			uint16;

/// This type is an integer with a size of 32 bits.
typedef int32_t				int32;
/// This type is an unsigned integer with a size of 32 bits.
typedef uint32_t			uint32;
/// This type is an integer with a size of 64 bits.
typedef int64_t				int64;
/// This type is an unsigned integer with a size of 64 bits.
typedef uint64_t			uint64;

namespace PyScript
{

class PyObjectPtr;


/**
 *	Collection of helper functions for converting between C++ and Python types.
 *	@see //bw/bigworld/branches/release/2/current/programming/bigworld/lib/pyscript/script.cpp
 */
namespace TypeConverter
{
	int setData( PyObject * pObj, 
		bool & rVal, const char * varName = "" );
	int setData( PyObject * pObj, 
		int  & rVal, const char * varName = "" );
	int setData( PyObject * pObj, uint & rVal, const char * varName = "" );

#ifdef __APPLE__
	int setData( PyObject * pObj, long & rVal, const char * varName = "" );
#endif

	int setData( PyObject * pObj, 
		float & rVal, const char * varName = "" );
	int setData( PyObject * pObj, double & rVal, const char * varName = "" );
	int setData( PyObject * pObj, int64 & rVal, const char * varName = "" );
	int setData( PyObject * pObj, uint64 & rVal, const char * varName = "" );
	int setData( PyObject * pObj, PyObject * & rVal,
		const char * varName = "" );
	int setData( PyObject * pObj, 
		PyObjectPtr & rPyObject,	const char * varName = "" );
	int setData( PyObject * pObj, std::string & rString,
		const char * varName = "" );
	int setData( PyObject * pObj, std::wstring & rString,
		const char * varName = "" );

	PyObject * getData( const bool data );
	PyObject * getData( const int data );
	PyObject * getData( const uint data );

#if defined( __clang__ )
	// Clang compiler realises long and int are distinct types, even when
	// they are the same length.
	PyObject * getData( const long data );
#endif // __clang__

	PyObject * getData( const float data );
	PyObject * getData( const double data );
	PyObject * getData( const int64 data );
	PyObject * getData( const uint64 data );

	PyObject * getData( const PyObject * data );
	PyObject * getData( const PyObjectPtr & data );
	PyObject * getData( const std::string & data );
	PyObject * getData( const std::wstring & data );
	PyObject * getData( const char * data );
	PyObject * getData( const wchar_t * data );

#define INT_ACCESSOR( INPUT_TYPE, COMMON_TYPE )					\
	inline PyObject * getData( const INPUT_TYPE data )			\
		{ return getData( COMMON_TYPE( data ) ); }				\
	inline int setData( PyObject * pObject, INPUT_TYPE & rInt,	\
						const char * varName = "" )				\
		{														\
			COMMON_TYPE value;									\
			int result = setData( pObject, value, varName );	\
			rInt = INPUT_TYPE( value );							\
			if (rInt != value )									\
			{													\
				PyErr_SetString( PyExc_TypeError, 				\
					"Integer is out of range" );				\
																\
				return -1;										\
			}													\
			return result;										\
		}


	INT_ACCESSOR( int8,  int );
	INT_ACCESSOR( int16, int );

	INT_ACCESSOR( uint8,  int );
	INT_ACCESSOR( uint16, int );


	/**
	 *	setData function for directly-addressable sequences (vectors, strings).
	 *	Lists would require a push_back-based implementation.
	 */
	template <class T, class SEQ> int setDataSequence( PyObject * pObj,
		SEQ & res, const char * varName )
	{
		if (!PySequence_Check( pObj ))
		{
			PyErr_Format( PyExc_TypeError, "%s must be set to a sequence of %s",
				varName, typeid(T).name() );
			return -1;
		}
		std::string eltVarName = varName; eltVarName += " element";
		Py_ssize_t sz = PySequence_Size( pObj );
		res.resize( sz );
		for (Py_ssize_t i = 0; i < sz; ++i)
		{
			PyObjectPtr pItem( PySequence_GetItem( pObj, i ), true );
			if (setData( pItem.get(), res[i], eltVarName.c_str() ) != 0)
			{
				return -1;
			}
		}
		return 0;
	}

	/// setData for vectors
	template <class T, class A> int setData( PyObject * pObj,
		std::vector<T,A> & res, const char * varName = "" )
	{
		return setDataSequence<T>( pObj, res, varName );
	}

	/// setData for basic_strings
	template <class C, class Tr, class A> int setData( PyObject * pObj,
		std::basic_string<C,Tr,A> & res, const char * varName = "" )
	{
		return setDataSequence<C>( pObj, res, varName );
	}

	/**
	 *	setData function for mappings (maps, multimaps).
	 */
	template <class K, class T, class MAP> int setDataMapping( PyObject * pObj,
		MAP & res, const char * varName )
	{
		if (!PyDict_Check( pObj ))	// using PyMapping API would be expensive
		{
			PyErr_Format( PyExc_TypeError, "%s must be set to a dict of %s: %s",
				varName, typeid(K).name(), typeid(T).name() );
			return -1;
		}
		std::string keyVarName = varName;
		std::string valueVarName = keyVarName;
		keyVarName += " key";
		valueVarName += " value";

		res.clear();
		Py_ssize_t pos = 0;
		PyObject * pKey, * pValue;
		while (PyDict_Next( pObj, &pos, &pKey, &pValue ))
		{
			std::pair<K,T> both;
			if (setData( pKey, both.first, keyVarName.c_str() ) != 0) return -1;
			if (setData( pValue, both.second, valueVarName.c_str() ) != 0) return -1;
			res.insert( both );
		}
		return 0;
	}

	// setData for maps
	template <class K, class T, class C, class A> int setData( PyObject * pObj,
		std::map<K,T,C,A> & res, const char * varName = "" )
	{
		return setDataMapping<K,T>( pObj, res, varName );
	}

	// setData for multimaps
	template <class K, class T, class C, class A> int setData( PyObject * pObj,
		std::multimap<K,T,C,A> & res, const char * varName = "" )
	{
		return setDataMapping<K,T>( pObj, res, varName );
	}

} // namespace TypeConverter


} // namespace PyScript


#endif // TYPE_CONVERTER_HPP

