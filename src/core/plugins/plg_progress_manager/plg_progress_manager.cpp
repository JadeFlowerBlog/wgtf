#include "progress_manager.hpp"

#include "generic_plugin/generic_plugin.hpp"


class ProgressManagerUIPlugin
	: public PluginMain
{
public:

	//==========================================================================
	ProgressManagerUIPlugin( IComponentContext & contextManager )
	{
	}

	//==========================================================================
	bool PostLoad( IComponentContext & contextManager )
	{
		return true;
	}

	//==========================================================================
	void Initialise( IComponentContext & contextManager )
	{
		progressManager_ = new ProgressManager();
		progressManager_->init( contextManager );
	}

	//==========================================================================
	bool Finalise( IComponentContext & contextManager )
	{
		delete progressManager_;
		progressManager_ = nullptr;

		return true;
	}

private:
	ProgressManager * progressManager_;

};

PLG_CALLBACK_FUNC( ProgressManagerUIPlugin )