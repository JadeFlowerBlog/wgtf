#pragma once
#include <memory>


class IClassDefinition;
class RefObjectId;

namespace PyScript
{
	class ScriptObject;
}


/**
 *	This class is for keeping a unique class definition for every Python object.
 */
struct IScriptObjectDefinitionRegistry
{
	/**
	 *	Registers a ScriptObject with the engine.
	 *	@pre object must not be null.
	 *  @return a IClassDefinition for the associated ScriptObject.
	 */
	virtual std::shared_ptr<IClassDefinition> getDefinition( const PyScript::ScriptObject& object ) = 0;

	/**
	 *	Get the ID for each ScriptObject.
	 *	@pre object must not be null.
	 *	@pre object must have been registered with getDefinition() first.
	 *  @return an ID for the associated ScriptObject.
	 */
	virtual const RefObjectId & getID( const PyScript::ScriptObject & object ) = 0;
};
