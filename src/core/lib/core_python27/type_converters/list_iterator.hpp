#pragma once
#ifndef PYTHON_LIST_ITERATOR_HPP
#define PYTHON_LIST_ITERATOR_HPP


#include "core_variant/collection.hpp"

#include "core_script/type_converter_queue.hpp"
#include "wg_pyscript/py_script_object.hpp"

#include <type_traits>


namespace PythonType
{
class IConverter;
} // namespace PythonType


typedef TypeConverterQueue< PythonType::IConverter,
	PyScript::ScriptObject > PythonTypeConverters;


namespace PythonType
{


template< typename T >
class ListIteratorImpl final : public CollectionIteratorImplBase
{
public:
	static const bool is_supported =
		std::is_convertible< T, PyScript::ScriptSequence >::value;

	static_assert( is_supported,
		"T must inherit from a PyScript::ScriptSequence type" );

	typedef T container_type;
	typedef typename container_type::size_type key_type;
	typedef Variant value_type;
	typedef ListIteratorImpl< T > this_type;

	ListIteratorImpl( const container_type & container,
		key_type index,
		const PythonTypeConverters & typeConverters );

	const container_type & container() const;
	/**
	 *	In Python if you pass in a negative index,
	 *	Python adds the length of the list to the index.
	 *	E.g. list[-1] gets the last item in the list
	 *	For this implementation, negative indexes should be checked and
	 *	converted to a positive one in the range start-end.
	 */
	virtual key_type index() const;
	virtual Variant key() const override;
	virtual Variant value() const override;
	virtual bool setValue( const Variant & value ) const override;
	virtual void inc() override;
	virtual bool equals( const CollectionIteratorImplBase & that ) const override;
	virtual CollectionIteratorImplPtr clone() const override;

private:
	container_type container_;
	key_type index_;
	const PythonTypeConverters & typeConverters_;
};


} // namespace PythonType


#endif // PYTHON_LIST_ITERATOR_HPP
