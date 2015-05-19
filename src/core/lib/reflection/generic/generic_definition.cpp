#include "generic_definition.hpp"

#include "reflection/generic/generic_object.hpp"
#include "reflection/i_definition_manager.hpp"
#include "reflection/utilities/definition_helpers.hpp"
#include "reflection/metadata/meta_impl.hpp"
#include "reflection/metadata/meta_utilities.hpp"
#include "reflection/object_handle.hpp"

const char * GenericDefinition::getName() const
{
	return name_.c_str();
}

//------------------------------------------------------------------------------
GenericDefinition::GenericDefinition( const char* name )
	:name_( name == nullptr
				? RefObjectId::generate().toString()
				: name )
{
}


//------------------------------------------------------------------------------
void GenericDefinition::init( IClassDefinitionModifier & modifier )
{
	modifier_ = &modifier;
}


//------------------------------------------------------------------------------
ObjectHandle GenericDefinition::create( const IClassDefinition & definition ) const
{
	auto & pInst = std::unique_ptr< GenericObject >( new GenericObject() );
	PolyStructDefinitionSetter setter( pInst.get(), &definition );
	pInst->definition_ = &definition;
	return ObjectHandle( pInst, &definition );
}


//------------------------------------------------------------------------------
ObjectHandle GenericDefinition::createBaseProvider(
	const ReflectedPolyStruct & polyStruct ) const
{
	auto pType =
		ReflectionUtilities::dynamicCast< GenericObject >( polyStruct );
	return createBaseProvider( polyStruct.getDefinition(), pType );
}


//------------------------------------------------------------------------------
ObjectHandle GenericDefinition::createBaseProvider(
	const IClassDefinition & definition, const void * pThis ) const
{
	return ObjectHandle(
		*static_cast< const GenericObject * >( pThis ), &definition );
}