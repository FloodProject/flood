/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "PCH.h"
#include "PluginManager.h"
#include "Plugin.h"
#include "Editor.h"

namespace vapor { namespace editor {

//-----------------------------------//

PluginManager::PluginManager(EditorFrame* frame)
	: editor(frame)
{ }

//-----------------------------------//

PluginManager::~PluginManager()
{
	foreach( const Plugin* plugin, plugins )
		delete plugin;
}

//-----------------------------------//

void PluginManager::registerPlugin( Plugin* plugin )
{
	if( !plugin )
		return;

	plugins.push_back(plugin);
}

//-----------------------------------//

void PluginManager::enablePlugin( Plugin* plugin )
{
	if( !plugin )
		return;
	
	PluginMetadata metadata = plugin->getMetadata();
	Log::info( "Enabling plugin: %s", metadata.name.c_str() );

	plugin->pluginEnabled = true;
	plugin->onPluginEnable();

	processTools( plugin, true );

	onPluginEnableEvent(plugin);
}

//-----------------------------------//

void PluginManager::disablePlugin( Plugin* plugin )
{
	if( !plugin )
		return;

	PluginMetadata metadata = plugin->getMetadata();
	Log::info( "Disabling plugin: %s", metadata.name.c_str() );

	processTools( plugin, false );

	plugin->pluginEnabled = false;
	plugin->doPluginDisable();

	onPluginDisableEvent(plugin);
}

//-----------------------------------//

void PluginManager::processTools( Plugin* plugin, bool enable )
{
	foreach( wxToolBarToolBase* tool, plugin->tools )
	{
		if( tool->IsSeparator() )
			continue;

		int id = tool->GetId();

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