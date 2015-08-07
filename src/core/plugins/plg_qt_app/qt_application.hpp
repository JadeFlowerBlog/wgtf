#ifndef QT_APPLICATION_HPP
#define QT_APPLICATION_HPP

#include "core_ui_framework/i_ui_application.hpp"
#include "core_ui_framework/layout_manager.hpp"

#include <memory>

class IQtFramework;
class QApplication;

class QtApplication : public Implements< IUIApplication >
{
public:
	QtApplication();
	virtual ~QtApplication();

	void initialise( IQtFramework * qtFramework );
	void finalise();
	void update();

	// IApplication
	int startApplication() override;
	void processEvents() override;
	void registerListener( IApplicationListener * listener ) override;
	void deregisterListener( IApplicationListener * listener ) override;

	// IUIApplication
	void addWindow( IWindow & window ) override;
	void addView( IView & view ) override;
	void addAction( IAction & action ) override;

	const Windows & windows() const override;
private:
	//void getCommandLine();
	//bool whiteSpace(char c);

	char** argv;
	int argc;

	IQtFramework * qtFramework_;
	std::unique_ptr< QApplication > application_;
	LayoutManager layoutManager_;
	std::vector< IApplicationListener * >	listeners_;
};

#endif//QT_APPLICATION_HPP