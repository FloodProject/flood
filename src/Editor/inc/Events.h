
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
class PluginManager;
class EditorFrame;

//-----------------------------------//

/**
 * Plugins can subscribe to these events and get notified when things
 * happen in the editor. For example, the Select tool needs to know
 * when a node is selected to show its bounding box.
 */

class Events : public wxEvtHandler
{
public:

	Events( EditorFrame* editor );
	virtual ~Events();

	// Add a plugin as an event listener.
	void addEventListener( Plugin* plugin );

	// Remove a plugin as an event listener.
	void removeEventListener( Plugin* plugin );
	
	// Entity selection event.
	void onEntitySelect( const EntityPtr& );

	// Entity unselection event.
	void onEntityUnselect( const EntityPtr& );

	// Component selection event.
	void onComponentSelect( const ComponentPtr& );

	// Component unselection event.
	void onComponentUnselect( const ComponentPtr& );

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

	// Scene load event.
	void onSceneLoad( const ScenePtr& scebe );

	// Scene update event.
	void onSceneUpdate();

	// Gets the current tool.
	ACESSOR(CurrentTool, int, toolId)

	// Gets/sets the current plugin.
	ACESSOR(CurrentPlugin, Plugin*, currentPlugin)

protected:

	// Handles plugin events.
	void onPluginEnableEvent(Plugin* plugin);
	void onPluginDisableEvent(Plugin* plugin);

	// Handles toolbar button clicks.
	void onToolbarButtonClick(wxCommandEvent& event);

	// Registers to the input events.
	void registerInputCallbacks();

	// Pre-handling of events from the toolbar.
	virtual bool TryBefore( wxEvent& event );

	// Current tool id.
	int toolId;

	// Keeps the main editor instance.
	EditorFrame* editor;

	// Keeps the plugin manager instance.
	PluginManager* pluginManager;

	// Current active plugin.
	Plugin* currentPlugin;

	// Global event listener plugins.
	std::vector<Plugin*> eventListeners;
};

//-----------------------------------//

} } // end namespaces
