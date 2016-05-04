#pragma once
#ifndef _REFLECTED_TREE_ITEM_NEW_HPP
#define _REFLECTED_TREE_ITEM_NEW_HPP

#include "core_data_model/abstract_item.hpp"
#include "core_dependency_system/di_ref.hpp"
#include "core_reflection/i_definition_manager.hpp"
#include "core_reflection/interfaces/i_reflection_controller.hpp"

class ReflectedTreeModelNew;

class IBaseProperty;
typedef std::shared_ptr< IBaseProperty > IBasePropertyPtr;

class IClassDefinition;
class ObjectHandle;
class PropertyAccessor;


/**
 *	Base class for adding a reflected item to a tree.
 */
class ReflectedTreeItemNew : public AbstractTreeItem
{
public:
	ReflectedTreeItemNew( IComponentContext & contextManager,
		ReflectedTreeItemNew * parent,
		const char * path ) ;
	ReflectedTreeItemNew( IComponentContext & contextManager,
		ReflectedTreeItemNew * parent,
		const std::string & path );
	virtual ~ReflectedTreeItemNew();

	virtual const ObjectHandle & getRootObject() const = 0;
	virtual const ObjectHandle & getObject() const = 0;
	virtual const IClassDefinition * getDefinition() const;

	virtual ReflectedTreeItemNew * getChild( size_t index ) const = 0;
	virtual int rowCount() const = 0;

	virtual bool isInPlace() const;

	virtual bool preSetValue( const PropertyAccessor & accessor,
		const Variant & value ) = 0;
	virtual bool postSetValue( const PropertyAccessor & accessor,
		const Variant & value ) = 0;

	const std::string & getPath() const;
	IReflectionController * getController() const;
	IDefinitionManager * getDefinitionManager() const;

	/**
	 *	Get data change/insert/remove signals from parent model.
	 *	@return signals or nullptr if item has not been added to a model.
	 */
	const ReflectedTreeModelNew * getModel() const;
	ReflectedTreeItemNew * getParent() const;

protected:
	ReflectedTreeItemNew * parent_;
	std::string path_;
	DIRef< IReflectionController > controller_;
	DIRef< IDefinitionManager > definitionManager_;

	typedef std::function< bool( const IBasePropertyPtr &, const std::string & ) >
		PropertyCallback;
	bool enumerateVisibleProperties( const PropertyCallback & callback ) const;

	/**
	 *	Used by parent to add change/insert/remove signals when item is
	 *	added/removed from the model.
	 *	@param signals or nullptr if item is being removed from the model.
	 */
	void setModel( const ReflectedTreeModelNew * pModel );
private:

	/**
	 *	Iterate through properties, ignoring items with MetaHidden.
	 */
	static bool enumerateVisibleProperties( ObjectHandle object,
		const IDefinitionManager & definitionManager,
		const std::string & inPlacePath,
		const PropertyCallback & callback );

	friend class ReflectedTreeModelNew;
	const ReflectedTreeModelNew * model_;
};

#endif //_REFLECTED_TREE_ITEM_NEW_HPP