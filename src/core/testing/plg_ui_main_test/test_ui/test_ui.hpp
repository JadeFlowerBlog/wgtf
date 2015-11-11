#ifndef TEST_UI_H
#define TEST_UI_H

#include <memory>
#include "core_dependency_system/depends.hpp"
#include "core_ui_framework/i_view.hpp"

class IAction;
class IUIApplication;
class IUIFramework;
class IWindow;
class IDataSource;
class IDataSourceManager;
class ICommandManager;
class IDefinitionManager;
class IReflectionController;
class IEnvManager;

class TestUI
	: Depends<
		IDefinitionManager,
		ICommandManager,
		IReflectionController,
		IDataSourceManager,
		IEnvManager >
	, public IViewEventListener
{
	typedef Depends<
		IDefinitionManager,
		ICommandManager,
		IReflectionController,
		IDataSourceManager,
		IEnvManager> DepsBase;
public:
	explicit TestUI( IComponentContext & context );
	~TestUI();

	void init( IUIApplication & uiApplication, IUIFramework & uiFramework );
	void fini();

	// IViewEventListener
	virtual void onFocusIn( IView* view ) override;
	virtual void onFocusOut( IView* view ) override;

private:
	void createActions( IUIFramework & uiFramework );

	void destroyActions();
	void destroyViews( size_t idx );

	void addActions( IUIApplication & uiApplication );
	void addViews( IUIApplication & uiApplication );

	void undo( IAction * action );
	void redo( IAction * action );
	bool canUndo( const IAction* action ) const;
	bool canRedo( const IAction* action ) const;

	void open();
	void close();
	void closeAll();
	bool canOpen() const;
	bool canClose() const;

	void createViews( IUIFramework & uiFramework, IDataSource* dataSrc, int envIdx );
	void removeViews( size_t idx );

	IUIApplication * app_;
	IUIFramework* fw_;

	std::unique_ptr< IAction > testOpen_;
	std::unique_ptr< IAction > testClose_;

	typedef std::vector< std::pair< IDataSource*, int > > DataSrcEnvPairs;
	DataSrcEnvPairs dataSrcEnvPairs_;

	typedef std::vector< std::pair< std::unique_ptr<IView>, int > > TestViews;
	TestViews test1Views_;
	TestViews test2Views_;
};


#endif // TEST_UI_H
