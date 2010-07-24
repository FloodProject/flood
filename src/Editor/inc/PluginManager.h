/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

namespace vapor { namespace editor {

class Plugin;
class EditorFrame;

//-----------------------------------//

/**
 * Responsible for managing all the plugins aswell as implementing
 * their lifecycle. Plugins should be registered with the manager
 * and enabled or disabled as needed during runtime.
 */

class PluginManager
{
	friend class PluginManagerFrame;

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

	// Switches to a different plugin.
	bool switchPlugin( int id );

	// Gets all the registered plugins.
	GETTER(Plugins, std::vector<Plugin*>, plugins)

	//-----------------------------------//

	// Selection events.
	void onNodeSelect( const NodePtr& );
	void onNodeUnselect( const NodePtr& );

	// Mouse input events.
	void onMouseMove( const MouseMoveEvent& );
	void onMouseDrag( const MouseDragEvent& );
	void onMousePress( const MouseButtonEvent& );
	void onMouseRelease( const MouseButtonEvent& );
	void onMouseEnter();
	void onMouseLeave();
	
	// Keyboard input events.
	void onKeyPress( const KeyEvent& );
	void onKeyRelease( const KeyEvent& );

protected:

	// Registers for input callbacks.
	void registerInputCallbacks();

	// Editor frame.
	EditorFrame* editorFrame;

	// Current tool.
	Plugin* currentTool;

	// Maps identifiers to tools.
	typedef std::map<int, Plugin*> PluginsMap;
	PluginsMap toolsIdsMap;

	// Keeps the known tools.
	std::vector<Plugin*> plugins;
};

//-----------------------------------//

} } // end namespaces