/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Core/API.h"
#include "Core/PluginManager.h"
#include "Core/Plugin.h"
#include "Core/Reflection.h"
#include "Core/Log.h"
#include "Core/Array.h"

NAMESPACE_CORE_BEGIN

//-----------------------------------//

PluginManager::PluginManager()
{

}

//-----------------------------------//

PluginManager::~PluginManager()
{
	for( size_t i = 0; i < plugins.size(); i++ )
	{
		Plugin* plugin = plugins[i];

		if( plugin->isEnabled() )
			LogAssert("Plugin should be disabled");

		Deallocate(plugin);
	}
}

//-----------------------------------//

void PluginManager::disablePlugins()
{
	// We disable plugins in an explicit step so allow plugins
	// being disabled to obtain other plugins and operate on them.

	for( size_t i = 0; i < plugins.size(); i++ )
	{
		Plugin* plugin = plugins[i];
		disablePlugin(plugin);
	}
}

//-----------------------------------//

Plugin* PluginManager::getPlugin(const String& name)
{
	for( size_t i = 0; i < plugins.size(); i++ )
	{
		Plugin* plugin = plugins[i];
		const PluginMetadata& metadata = plugin->getMetadata();
		if(metadata.name == name) return plugin;
	}

	return nullptr;
}

//-----------------------------------//

Plugin* PluginManager::getPluginFromClass(const Class* klass)
{
	for( size_t i = 0; i < plugins.size(); i++ )
	{
		Plugin* plugin = plugins[i];
		if(ClassGetType(plugin) == klass) return plugin;
	}

	return nullptr;
}

//-----------------------------------//

void PluginManager::registerPlugins(const Array<Plugin*>& plugins)
{
	for(auto plugin : plugins)
		registerPlugin(plugin);
}

//-----------------------------------//

void PluginManager::registerPlugin( Plugin* plugin )
{
	if( !plugin ) return;
	plugins.pushBack(plugin);
}

//-----------------------------------//

void PluginManager::enablePlugin( Plugin* plugin )
{
	if( !plugin ) return;
	
	PluginMetadata metadata = plugin->getMetadata();
	LogInfo( "Enabling plugin: %s", metadata.name.c_str() );

	plugin->enabled = true;
	plugin->onPluginEnable();

	onPluginEnableEvent(plugin);
}

//-----------------------------------//

void PluginManager::disablePlugin( Plugin* plugin )
{
	if( !plugin ) return;
	if( !plugin->isEnabled() ) return;

	PluginMetadata metadata = plugin->getMetadata();
	LogInfo( "Disabling plugin: %s", metadata.name.c_str() );

	onPluginDisableEvent(plugin);

	plugin->enabled = false;
	plugin->doPluginDisable();
}

//-----------------------------------//

NAMESPACE_CORE_END