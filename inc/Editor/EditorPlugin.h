/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Core/Plugin.h"
#include "Input/MouseEvents.h"
#include "Input/KeyboardEvents.h"
#include "Document.h"
#include "UndoOperation.h"

FWD_DECL_INTRUSIVE(Scene)
FWD_DECL_INTRUSIVE(Entity)
FWD_DECL_INTRUSIVE(Resource)
FWD_DECL_INTRUSIVE(Session)

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

class EditorFrame;
class EditorPlugin;

/**
 * Editor extensions are used to extend the editor with new functionality.
 * The editor has a couple extension points, and each plugins registers
 * custom extensions.
 */

class API_EDITOR EditorExtension : public Extension
{
public:

	EditorExtension();

	// Plugin that owns the extension.
	EditorPlugin* plugin;
};

//-----------------------------------//

class API_EDITOR ToolExtension : public EditorExtension
{
public:

	ToolExtension();

	// Gets metadata about this extension.
	ExtensionMetadata* getMetadata() OVERRIDE;

	// Sets the toolbar.
	void setToolbar(wxAuiToolBar*);

	// Context-specific toolbar.
	wxAuiToolBar* toolbar;
	
	// Item that triggers the toolbar.
	wxAuiToolBarItem* item;
};

typedef std::vector<ToolExtension> ToolExtensions;

//-----------------------------------//

class API_EDITOR DocumentExtension : public EditorExtension
{
public:

	DocumentExtension();

	// Gets metadata about this extension.
	ExtensionMetadata* getMetadata() OVERRIDE;

	// Gets the file extensions of the documents.
	//virtual String getFileExtensions() = 0;
};

typedef std::vector<DocumentExtension> DocumentExtensions;

//-----------------------------------//

/**
 * An editor needs to provide different kind of tools to the user,
 * so to be flexible and allow extensibility each tool behaviours and
 * functionality is provided by a custom type derived from this class.
 *
 * As each tool can be loaded/unloaded at runtime, there is a tool
 * lifecycle you must implement. Most of the work is done by asking
 * the editor to execute actions in the different events/callbacks.
 */

REFLECT_DECLARE_CLASS(EditorPlugin)

class API_EDITOR EditorPlugin : public Plugin
{
	REFLECT_DECLARE_OBJECT(EditorPlugin)

	friend class PluginManager;

public:

	EditorPlugin();
	virtual ~EditorPlugin();

	// Finds if there is a tool with given id.
	ToolExtension* findTool( wxAuiToolBarItem* tool );

	// Finds if there is a tool with given id.
	ToolExtension* findToolById( int toolId );

	// Base callbacks.
	virtual void onPluginRegistered() {}
	virtual void onPluginUpdate() {}

	// Plugin tool callback.
	virtual void onToolSelect( int id ) {}
	virtual void onToolUnselect( int id ) {}
	virtual void onToolNone() {}

	// Document callbacks.
	virtual void onDocumentCreate( Document& document ) {}
	virtual void onDocumentDestroy( Document& document ) {}
	virtual void onDocumentSelect( Document& ) {}
	virtual void onDocumentUnselect( Document& ) {}

	// Scene load callback.
	virtual void onSceneLoad( const ScenePtr& scene ) {}
	virtual void onSceneUnload( const ScenePtr& scene ) {}
	virtual void onSceneUpdate() {}

	// Entity selection callback.
	virtual void onEntitySelect( const EntityPtr& ) {}
	virtual void onEntityUnselect( const EntityPtr& ) {}

	// Component selection callback.
	virtual void onComponentSelect( const ComponentPtr& ) {}
	virtual void onComponentUnselect( const ComponentPtr& ) {}

	// Resource selection event.
	virtual void onResourceSelect( const ResourcePtr& ) {}
	virtual void onResourceUnselect( const ResourcePtr& ) {}

	// Mouse callback.
	virtual void onMouseMove( const MouseMoveEvent& ) {}
	virtual void onMouseDrag( const MouseDragEvent& ) {}
	virtual void onMouseButtonPress( const MouseButtonEvent& ) {}
	virtual void onMouseButtonRelease( const MouseButtonEvent& ) {}
	virtual void onMouseEnter() {}
	virtual void onMouseLeave() {}
	
	// Key callback.
	virtual void onKeyPress(const KeyEvent&) {}
	virtual void onKeyRelease(const KeyEvent&) {}

	// Server callbacks.
	virtual void onServerConnect(const SessionPtr&) {}
	virtual void onServerDisconnect(const SessionPtr&) {}

	// Undo/redo operation events.
	virtual void onUndoOperation(const UndoOperationPtr&) {}
	virtual void onRedoOperation(const UndoOperationPtr&) {}

	// Returns if the tool is from the plugin.
	bool hasTool(int id) const;

	// Returns if the current tool selected is from the plugin.
	bool isToolSelected() const;

protected:

	// Disables plugins.
	void doPluginDisable() OVERRIDE;
	
	// Registers a new tool in the plugin.
	void addTool( ToolExtension& tool, bool addToMenu = false );
	void addTool( wxAuiToolBarItem* tool, bool addToMenu = false );

	// Removes all the registered tools.
	void removeTools();

	// Removes a page from the main notebook.
	void removePage( wxWindow* page );

	// Keeps track of all the registered tools.
	ToolExtensions tools;

	// Keeps track of document extensions.
	DocumentExtensions documents;

	// Editor frame.
	EditorFrame* editor;
};

// Returns if the current tool selected is the given one.
template<typename T> bool IsToolSelected(T tool)
{
	int id = (int) tool;
	int current = GetEditor().getEventManager()->getCurrentToolId();

	return id == current;
}

//-----------------------------------//

NAMESPACE_EDITOR_END