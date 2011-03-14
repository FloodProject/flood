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
#include "Reflection.h"

namespace vapor { namespace editor {

//-----------------------------------//

PluginManager::PluginManager() { }

//-----------------------------------//

PluginManager::~PluginManager()
{
	for( size_t i = 0; i < plugins.size(); i++ )
	{
		Plugin* plugin = plugins[i];
		if(plugin->enabled) disablePlugin(plugin);

		delete plugin;
	}
}

//-----------------------------------//

Plugin* PluginManager::getPlugin(const std::string& name)
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

void PluginManager::scanPlugins()
{
	Class& type = Plugin::getStaticType();

	for( size_t i = 0; i < type.childs.size(); i++ )
	{
		Class* child = type.childs[i];
		if( !child ) continue;

		Plugin* plugin = (Plugin*) child->createInstance();
		if(!plugin) continue;

		registerPlugin(plugin);
		
		const PluginMetadata& metadata = plugin->getMetadata();
		if(metadata.startEnabled) enablePlugin(plugin);
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
		wxAuiToolBarItem* tool = plugin->tools[i];
	
		//if( tool->IsSeparator() )
			//continue;

		int id = tool->GetId();

		if( id == -1 ) continue;

		PluginToolsMap::iterator it = tools.find(id);
	
		if( enable )
			// assert( it == tools.end() );
			tools[id] = plugin;
		else
			// assert( it != tools.end() );
			tools.erase(it);
	}
}

//-----------------------------------//

} } // end namespaces