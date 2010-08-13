
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

	// Pre-handling of events from the toolbar.
	virtual bool TryBefore( wxEvent& event );
	
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

	// Scene update events.
	void onSceneUpdate();

	// Current active plugin.
	Plugin* currentPlugin;

protected:

	// Handles plugin events.
	void onPluginEnableEvent(Plugin* plugin);
	void onPluginDisableEvent(Plugin* plugin);

	// Handles toolbar button clicks.
	void onToolbarButtonClick(wxCommandEvent& event);

	// Registers to the input events.
	void registerInputCallbacks();

	// Keeps the main editor instance.
	EditorFrame* editor;

	// Keeps the plugin manager instance.
	PluginManager* pluginManager;

	// Global event listener plugins.
	std::vector<Plugin*> eventListeners;
};

//-----------------------------------//

} } // end namespaces
