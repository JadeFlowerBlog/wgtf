#include "set_reflectedproperty_command.hpp"

#include "core_variant/variant.hpp"
#include "command_system_property_setter.hpp"
#include "core_reflection/i_object_manager.hpp"
#include "core_command_system/i_command_manager.hpp"

//==============================================================================
const char * ReflectedPropertyCommandArgument::s_ContextId = "PropertyContextId";
const char * ReflectedPropertyCommandArgument::s_PropertyPath = "PropertyPath";
const char * ReflectedPropertyCommandArgument::s_PropertyValue = "PropertyValue";


//==============================================================================
const char * ReflectedPropertyCommandArgument::contextIdPropertyName()
{
	return s_ContextId;
}


//==============================================================================
const char * ReflectedPropertyCommandArgument::pathPropertyName()
{
	return s_PropertyPath;
}


//==============================================================================
const char * ReflectedPropertyCommandArgument::valuePropertyName()
{
	return s_PropertyValue;
}


//==============================================================================
ReflectedPropertyCommandArgument::ReflectedPropertyCommandArgument()
	: contextId_("")
	, propertyPath_("")
{
}


//==============================================================================
const RefObjectId & ReflectedPropertyCommandArgument::getContextId() const
{
	return contextId_;
}


//==============================================================================
const char * ReflectedPropertyCommandArgument::getPropertyPath() const
{
	return propertyPath_.c_str();
}


//==============================================================================
const Variant & ReflectedPropertyCommandArgument::getPropertyValue() const
{
	return value_;
}


//==============================================================================
void ReflectedPropertyCommandArgument::setContextId( const RefObjectId & id )
{
	contextId_ = id;
}


//==============================================================================
void ReflectedPropertyCommandArgument::setPath( const char * path )
{
	propertyPath_ = path;
}


//==============================================================================
void ReflectedPropertyCommandArgument::setValue( const Variant & value )
{
	value_ = value;
}


//==============================================================================
const char * SetReflectedPropertyCommand::getId() const
{
	static const char * s_Id = getClassIdentifier<SetReflectedPropertyCommand>();
	return s_Id;
}
//==============================================================================
ObjectHandle SetReflectedPropertyCommand::execute(
	const ObjectHandle & arguments ) const
{
	auto commandArgs =
		arguments.getBase< ReflectedPropertyCommandArgument >();
	auto objManager = arguments.getDefinition()->getDefinitionManager()->getObjectManager();
	assert( objManager != nullptr );
	const ObjectHandle & object = objManager->getObject( commandArgs->getContextId() );
	PropertyAccessor property = object.getDefinition()->bindProperty( commandArgs->getPropertyPath(), object );
	if (property.isValid() == false)
	{
		//Can't set
		return ObjectHandle::makeStorageBackedProvider( CommandErrorCode::INVALID_ARGUMENTS );
	}
	const Variant & data = commandArgs->getPropertyValue();
	// special handle for polystruct
	if (ReflectionUtilities::isPolyStruct( property ))
	{
		ObjectHandle provider;
		
		if (data.tryCast< ObjectHandle >( provider ))
		{
			auto classDefinition = provider.getBase< IClassDefinition >();
			if (classDefinition != nullptr)
			{
				ObjectHandle created;
				created = classDefinition->create();
				if(!property.setValue( created ))
				{
					return ObjectHandle::makeStorageBackedProvider( CommandErrorCode::INVALID_VALUE );
				}
				return nullptr;
			}
		}
	}
	// handle for property value inherits from IClassDefinition type
	else if (property.getStructDefinition() != nullptr)
	{
		// if the value's definition is not matching to 
		// target definition, do not set the value
		auto value = property.getValue();
		ObjectHandle baseProvider;
		value.tryCast( baseProvider );
		if (baseProvider.isValid())
		{
			auto desDef = baseProvider.getDefinition();
			if (desDef != nullptr)
			{
				ObjectHandle provider;
				data.tryCast( provider );
				if (!provider.isValid())
				{
					return ObjectHandle::makeStorageBackedProvider( CommandErrorCode::INVALID_VALUE );
				}
				auto def = provider.getDefinition();
				if (def == nullptr)
				{
					return ObjectHandle::makeStorageBackedProvider( CommandErrorCode::INVALID_VALUE );
				}
				// check generic definition
				if (desDef->isGeneric())
				{
					if (!def->isGeneric())
					{
						return ObjectHandle::makeStorageBackedProvider( CommandErrorCode::INVALID_VALUE );
					}
				}
				else
				{
					if(!def->canBeCastTo( *desDef ))
					{
						return ObjectHandle::makeStorageBackedProvider( CommandErrorCode::INVALID_VALUE );
					}
				}

			}
		}
	}

	bool br = property.setValue( data );
	if (!br)
	{
		return ObjectHandle::makeStorageBackedProvider( CommandErrorCode::INVALID_VALUE );
	}
	return nullptr;
}