#pragma once

#include "core_ui_framework/i_view.hpp"
#include "core_generic_plugin/interfaces/i_component_context.hpp"
#include "core_reflection/object_handle.hpp"
#include "core_dependency_system/depends.hpp"
#include "core_python_script/i_scripting_engine.hpp"


#include <memory>


class IDefinitionManager;
class IReflectionController;
class IUIFramework;
class IUIApplication;
class PythonContextObject;


#define DEPENDS_ON_CLASSES \
	IDefinitionManager, \
	IReflectionController, \
	IPythonScriptingEngine, \
	IUIFramework, \
	IUIApplication


/// Main class containing the Python related classes and panel.
class PythonPanel: Depends<DEPENDS_ON_CLASSES>
{
	typedef Depends<DEPENDS_ON_CLASSES> DepsBase;


public:
	PythonPanel( IComponentContext& context );


	/// Initialise panel, and all objects it needs.
	void initialise();


	/// Removes panel, and destroys objects.
	void finalise();


private:
	/// Create the context object to provide access to the Python references from QML.
	/// Also registers definitions of reflected objects with the reflection system.
	/// @return true if successful, false if not.
	bool createContextObject();


	/// Create references to Python objects and add these to the context object.
	/// @return true if successful, false if not.
	bool createPythonObjects();


	/// Create the panel and add it to the window. (The window exists in a different plugin)
	/// @return true if successful, false if not.
	bool addPanel();


	/// Remove the panel from the window.
	void removePanel();


	/// Destroy context objects and Python references.
	void destroyPythonObjects();


	IComponentContext& context_;
	std::unique_ptr<IView> pythonView_;
	ObjectHandleT<PythonContextObject> contextObject_;
};
