#pragma once
#ifndef _REFLECTED_GROUP_ITEM_NEW_HPP
#define _REFLECTED_GROUP_ITEM_NEW_HPP

#include "reflected_tree_item_new.hpp"

#include <memory>

class MetaGroupObj;

class ReflectedGroupItemNew : public ReflectedTreeItemNew
{
public:
	ReflectedGroupItemNew( IComponentContext & contextManager,
		const MetaGroupObj * groupObj,
		ReflectedTreeItemNew * parent,
		const std::string & inPlacePath );
	virtual ~ReflectedGroupItemNew();

	// AbstractItem
	virtual Variant getData( int column, size_t roleId ) const override;
	virtual bool setData( int column, size_t roleId, const Variant & data ) override;

	// ReflectedTreeItemNew
	virtual const ObjectHandle & getRootObject() const override;
	virtual const ObjectHandle & getObject() const override;
	virtual ReflectedTreeItemNew * getChild( size_t index ) const override;

	virtual int rowCount() const override;

	virtual bool preSetValue( const PropertyAccessor & accessor,
		const Variant & value ) override;
	virtual bool postSetValue( const PropertyAccessor & accessor,
		const Variant & value ) override;

private:
	class Implementation;
	std::unique_ptr< Implementation > impl_;

	typedef std::vector< Variant > Variants;
	void getChildValues( Variants & outChildValues ) const;

	typedef std::function< bool( ReflectedTreeItemNew & ) > ReflectedItemCallback;
	void enumerateChildren( const ReflectedItemCallback & callback ) const;
};

#endif //_REFLECTED_GROUP_ITEM_NEW_HPP