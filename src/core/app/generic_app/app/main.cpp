#include "wg_memory/memory_overrides.hpp"

#include "generic_plugin_system/generic_plugin_manager.hpp"
#include "generic_plugin_system/folder_plugin_loader.hpp"
#include "generic_plugin_system/config_plugin_loader.hpp"
#include "generic_plugin_system/interfaces/i_application.hpp"
#include "memory_plugin_context_creator.hpp"

#include <shlwapi.h>
#include <ShellAPI.h>
  
namespace
{

bool getPlugins (std::vector< std::wstring >& plugins)
{
	int argumentCount = 0;
	LPWSTR * arguments = 
		::CommandLineToArgvW( ::GetCommandLineW(), &argumentCount );

	LPWSTR configFile = NULL;

	for (int i = 0; i < argumentCount; ++i)
	{
		if (::wcscmp( arguments[i], L"--config" ) == 0)
		{
			++i;
			if (i < argumentCount)
			{
				configFile = arguments[i];
			}
		}
	}

	WCHAR path[MAX_PATH];
	::GetModuleFileNameW( NULL, path, MAX_PATH );
	::PathRemoveFileSpecW( path );

	if (configFile != NULL)
	{
		::PathAppendW( path, L"plugins\\" );
		::PathAppendW( path, configFile );
		return ConfigPluginLoader::getPlugins( plugins, path );
	}
	else
	{
		::PathAppendW( path, L"plugins\\" );
		
		return 
			ConfigPluginLoader::getPlugins( 
				plugins, std::wstring( path ) + L"plugins.txt" ) ||
			FolderPluginLoader::getPluginsCustomPath( plugins, path );
	}
}

}

int WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, 
			 LPSTR lpCmdLine, int nShowCmd )
{
	std::vector< std::wstring > plugins;
	if (!getPlugins( plugins ) || plugins.empty())
	{
		return 2; // failed to find any plugins!
	}

	int result = 1;

	{
		GenericPluginManager pluginManager;
		IPluginContextManager & contextManager =
			pluginManager.getContextManager();
		contextManager.getGlobalContext()->registerInterface(
			new MemoryPluginContextCreator );

		pluginManager.loadPlugins( plugins );
		
		IApplication* application =
			contextManager.getGlobalContext()->queryInterface< IApplication >();
		if (application != NULL)
		{
			result = application->startApplication();
		}
	}
	return result;
}