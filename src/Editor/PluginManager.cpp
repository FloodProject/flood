/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Editor/API.h"
#include "PluginManager.h"
#include "Plugin.h"
#include "Editor.h"
#include "Core/Reflection.h"

NAMESPACE_EDITOR_BEGIN

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
		disablePlugin(plugin);
		Deallocate(plugin);
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

static bool SortPluginsCallback(Plugin* a, Plugin* b)
{
	int priorityA = a->getMetadata().priority;
	int priorityB = b->getMetadata().priority;

	return priorityA < priorityB;
}

void PluginManager::sortPlugins(std::vector<Plugin*>& plugins)
{
	// Sort the plugins by priority.
	std::sort(plugins.begin(), plugins.end(), SortPluginsCallback);
}

//-----------------------------------//

void PluginManager::scanPlugins(std::vector<Plugin*>& plugins)
{
	Class* klass = ReflectionGetType(Plugin);

	for( size_t i = 0; i < klass->childs.size(); i++ )
	{
		Class* child = klass->childs[i];
		if( !child ) continue;

		Plugin* plugin = (Plugin*) ClassCreateInstance(child, AllocatorGetHeap());
		if(!plugin) continue;

		plugins.push_back(plugin);
	}
}

//-----------------------------------//

void PluginManager::registerPlugins(const std::vector<Plugin*>& plugins)
{
	for( size_t i = 0; i < plugins.size(); i++ )
	{
		Plugin* plugin = plugins[i];
		registerPlugin(plugin);
		
		const PluginMetadata& metadata = plugin->getMetadata();
		
		if(metadata.startEnabled)
			enablePlugin(plugin);
	}
}

//-----------------------------------//

void PluginManager::registerPlugin( Plugin* plugin )
{
	if( !plugin ) return;

	plugin->editor = &GetEditor();
	plugins.push_back(plugin);
}

//-----------------------------------//

void PluginManager::enablePlugin( Plugin* plugin )
{
	if( !plugin ) return;
	
	PluginMetadata metadata = plugin->getMetadata();
	LogInfo( "Enabling plugin: %s", metadata.name.c_str() );

	plugin->enabled = true;
	plugin->onPluginEnable();

	processTools( plugin, true );

	onPluginEnableEvent(plugin);
}

//-----------------------------------//

void PluginManager::disablePlugin( Plugin* plugin )
{
	if( !plugin ) return;
	if( !plugin->isEnabled() ) return;

	PluginMetadata metadata = plugin->getMetadata();
	LogInfo( "Disabling plugin: %s", metadata.name.c_str() );

	processTools( plugin, false );

	plugin->enabled = false;
	plugin->doPluginDisable();

	onPluginDisableEvent(plugin);
}

//-----------------------------------//

void PluginManager::processTools( Plugin* plugin, bool enable )
{
	for( size_t i = 0; i < plugin->tools.size(); i++ )
	{
		wxAuiToolBarItem* tool = plugin->tools[i].item;
		tool->SetUserData((long) plugin);
	}
}

//-----------------------------------//

NAMESPACE_EDITOR_END