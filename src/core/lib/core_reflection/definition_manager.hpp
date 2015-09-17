#ifndef DEFINITION_MANAGER_HPP
#define DEFINITION_MANAGER_HPP

#include <unordered_map>

#include "i_definition_manager.hpp"
#include "core_dependency_system/i_interface.hpp"

#include "wg_types/hashed_string_ref.hpp"

/**
 * DefinitionManager
 */
class DefinitionManager 
	: public Implements< IDefinitionManager >
{ 
public:
	// IDefinitionManager
	virtual IClassDefinition * getDefinition( const char * name ) const override;
	IClassDefinitionDetails * createGenericDefinition(
		const char * name ) const override;

	virtual void getDefinitionsOfType( const IClassDefinition * definition,
		std::vector< IClassDefinition * > & o_Definitions ) const override;

	virtual void getDefinitionsOfType( const std::string & type,
		std::vector< IClassDefinition * > & o_Definitions ) const override;

	void registerPropertyAccessorListener( std::shared_ptr< PropertyAccessorListener > & listener ) override;
	void deregisterPropertyAccessorListener( std::shared_ptr< PropertyAccessorListener > & listener ) override;
	const PropertyAccessorListeners & getPropertyAccessorListeners() const override;

	virtual IObjectManager * getObjectManager() const override;

	virtual IClassDefinition * registerDefinition(
		IClassDefinitionDetails * definition,
		IClassDefinitionModifier ** o_Modifier ) override;
	virtual bool deregisterDefinition( IClassDefinition * definition ) override;

	bool serializeDefinitions( IDataStream & dataStream ) override;
	bool deserializeDefinitions( IDataStream & dataStream ) override;

	DefinitionManager( IObjectManager & objectManager );
	virtual ~DefinitionManager();

private:
	typedef std::unordered_map< HashedStringRef, IClassDefinition * > ClassDefCollection;
	ClassDefCollection definitions_;

	void getDefinitionsOfType(
		IClassDefinition * definition,
		std::vector< IClassDefinition * > & o_Definitions,
		size_t startIndex ) const;

	PropertyAccessorListeners listeners_;
	IObjectManager & objectManager_;
};


#endif // DEFINITION_MANAGER_HPP
