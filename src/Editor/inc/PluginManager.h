/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

namespace vapor { namespace editor {

//-----------------------------------//

class Plugin;
class EditorFrame;

typedef std::map<int, Plugin*> PluginToolsMap;

//-----------------------------------//

/**
 * Responsible for managing all the plugins aswell as implementing
 * their lifecycle. Plugins should be registered with the manager
 * and enabled or disabled as needed dpathng runtime.
 */

class PluginManager
{
public:

	PluginManager( EditorFrame* frame );
	~PluginManager();

	// Registers a new plugin.
	void registerPlugin( Plugin* plugin );

	// Enables the plugin.
	void enablePlugin( Plugin* plugin );

	// Disables the plugin.
	void disablePlugin( Plugin* plugin );

	// Returns if plugin is enabled.
	bool isPluginEnabled( Plugin* plugin ) const;

	// Scans for plugins.
	//void scanPlugins();

	// Gets all the registered plugins.
	GETTER(Plugins, std::vector<Plugin*>, plugins)

	// Gets all the registered tools.
	GETTER(Tools, const PluginToolsMap&, tools)

	// Plugin events callback.
	Event1<Plugin*> onPluginEnableEvent;
	Event1<Plugin*> onPluginDisableEvent;

protected:
	
	// Processes tools from a plugin into tools map.
	void processTools( Plugin* plugin, bool enable );

	// Editor frame.
	EditorFrame* editor;

	// Maps identifiers to tools.
	PluginToolsMap tools;

	// Keeps the known tools.
	std::vector<Plugin*> plugins;
};

//-----------------------------------//

} } // end namespaces