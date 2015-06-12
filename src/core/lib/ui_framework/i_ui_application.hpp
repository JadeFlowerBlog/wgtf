#ifndef I_UI_APPLICATION_HPP
#define I_UI_APPLICATION_HPP

#include "dependency_system/i_interface.hpp"
#include "generic_plugin/interfaces/i_application.hpp"
#include "layout_hint.hpp"

class IAction;
class IComponent;
class IWindow;
class IView;

class IUIApplication : public Implements< IApplication >
{
public:
	virtual ~IUIApplication() {}

	virtual void addWindow( IWindow & window ) = 0;
	virtual void addView( IView & view ) = 0;
	virtual void addAction( IAction & action ) = 0;
};

#endif//I_UI_APPLICATION_HPP