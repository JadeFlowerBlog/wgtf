#ifndef GENERIC_DEFINITION_HELPER_HPP
#define GENERIC_DEFINITION_HELPER_HPP

#include "core_reflection/interfaces/i_definition_helper.hpp"

class GenericDefinitionHelper : public IDefinitionHelper
{
public:
	virtual TypeId typeId() const override;

	virtual IClassDefinition * getDefinition( const ObjectHandle & object ) const override;

};

#endif

