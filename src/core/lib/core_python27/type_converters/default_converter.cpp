#include "pch.hpp"
#include "default_converter.hpp"
#include "core_python27/defined_instance.hpp"
#include "core_variant/variant.hpp"
#include "wg_pyscript/py_script_object.hpp"


namespace PythonType
{


DefaultConverter::DefaultConverter( IComponentContext & context )
	: context_( context )
{
}


bool DefaultConverter::toVariant( const PyScript::ScriptObject & inObject,
	Variant & outVariant,
	const PyScript::ScriptObject & parentObject,
	const std::string & childPath ) const
{
	// Default for anything that didn't match another type converter
	outVariant = ReflectedPython::DefinedInstance::findOrCreate( context_,
		inObject,
		parentObject,
		childPath );
	return true;
}


bool DefaultConverter::toScriptType( const Variant & inVariant,
	PyScript::ScriptObject & outObject ) const
{
	if (inVariant.typeIs<Variant::traits<ObjectHandle>::storage_type>())
	{
		ObjectHandle handle = inVariant.value<ObjectHandle>();
		auto contents = handle.getBase<ReflectedPython::DefinedInstance>();

		if (contents == nullptr)
		{
			return false;
		}

		outObject = contents->pythonObject();
		return true;
	}

	return false;
}


} // namespace PythonType
