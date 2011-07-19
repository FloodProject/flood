
/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "EditorPlugin.h"
#include "Input/InputManager.h"
#include "Input/Mouse.h" 
#include "Input/Keyboard.h"

FWD_DECL_INTRUSIVE(Resource)

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

class EditorFrame;
class Document;

class PluginTool;

/**
 * Plugins can subscribe to these events and get notified when things
 * happen in the editor. For example, the select tool needs to know
 * when an entity is selected to show its bounding box.
 */

typedef std::vector<EditorPlugin*> EventListeners;

class API_EDITOR Events : public wxEvtHandler
{
public:

	Events();
	virtual ~Events();

	void disconnectPluginListeners();

	// Add/remove a plugin as an event listener.
	void addEventListener( EditorPlugin* plugin );
	void removeEventListener( EditorPlugin* plugin );

	// Document creation event.
	void onDocumentCreate( Document& document );
	void onDocumentDestroy( Document& document );

	// Document selection event.
	void onDocumentSelect( Document& document );
	void onDocumentUnselect( Document& document );

	// Entity selection event.
	void onEntitySelect( const EntityPtr& );
	void onEntityUnselect( const EntityPtr& );

	// Component selection event.
	void onComponentSelect( const ComponentPtr& );
	void onComponentUnselect( const ComponentPtr& );

	// Resource selection event.
	void onResourceSelect( const ResourcePtr& );
	void onResourceUnselect( const ResourcePtr& );

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
	void onSceneLoad( const ScenePtr& scene );
	void onSceneUnload( const ScenePtr& scene );
	void onSceneUpdate();

	// Gets the current tool.
	ACESSOR(CurrentTool, int, toolId)

	// Gets/sets the current plugin.
	ACESSOR(CurrentPlugin, EditorPlugin*, currentPlugin)

	// Switches the current plugin.
	void setTool(EditorPlugin* plugin, PluginTool* tool);

protected:

	// Handles plugin events.
	void onPluginEnableEvent(Plugin* plugin);
	void onPluginDisableEvent(Plugin* plugin);

	// Handles toolbar button clicks.
	void onToolbarButtonClick(wxCommandEvent& event);

	// Registers to the input events.
	void registerInputCallbacks();

	// Pre-handling of events from the toolbar.
	virtual bool TryBefore( wxEvent& event ) OVERRIDE;

	// Current tool id.
	int toolId;

	// Current active plugin.
	EditorPlugin* currentPlugin;

	// Global event listener plugins.
	EventListeners eventListeners;
};

//-----------------------------------//

NAMESPACE_EDITOR_END
