#include "automation/interfaces/automation_interface.hpp"

#include <ctime>

#include "generic_plugin/generic_plugin.hpp"

class Automation : public Implements< AutomationInterface >
{
public:
	Automation()
		: startTime_( std::clock() )
	{
	}

	virtual bool timedOut() override
	{
		const auto currentTime = std::clock();
		const auto dTime = currentTime - startTime_;
		const auto dTimeS = dTime / CLOCKS_PER_SEC;
		// TODO make this configurable
		const double TIMEOUT_SECONDS = 30.0;
		return (dTimeS > TIMEOUT_SECONDS);
	}

private:
	const clock_t startTime_;
};

class AutomationPlugin
	: public PluginMain
{
public:
	AutomationPlugin( IComponentContext & contextManager )
	{
	}

	virtual bool PostLoad( IComponentContext & contextManager ) override
	{
		contextManager.registerInterface( new Automation() );
		return true;
	}
};

PLG_CALLBACK_FUNC( AutomationPlugin )

