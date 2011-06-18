/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Input/MouseEvents.h"
#include "Input/KeyboardEvents.h"
#include "Document.h"

FWD_DECL_INTRUSIVE(Scene)
FWD_DECL_INTRUSIVE(Entity)
FWD_DECL_INTRUSIVE(Resource)

namespace vapor { namespace editor {

//-----------------------------------//

struct PluginMetadata
{
	PluginMetadata() : startEnabled(true), priority(1000) {}

	// Name of this plugin.
	String name;

	// Description of this plugin.
	String description;

	// Author of this plugin.
	String author;

	// Version of this plugin.
	String version;

	// Enabled by default.
	bool startEnabled;

	// Startup priority.
	int priority;
};

//-----------------------------------//

class EditorFrame;
class PluginManager;

struct PluginTool
{
	PluginTool() : toolbar(nullptr), item(nullptr) {}

	// Sets the toolbar.
	void setToolbar(wxAuiToolBar*);

	// Context-specific toolbar.
	wxAuiToolBar* toolbar;
	
	// Item that triggers the toolbar.
	wxAuiToolBarItem* item;
};

typedef std::vector<PluginTool> PluginTools;


/**
 * A 3D editor needs to provide different kind of tools to the user,
 * so to be flexible and allow extensibility each tool behaviours and
 * functionality is provided by a custom type derived from this class.
 * As each tool can be loaded/unloaded at runtime, there is a tool
 * lifecycle you must implement. Most of the work is done by asking
 * the editor to execute actions in the different events/callbacks.
 */

REFLECT_DECLARE_CLASS(Plugin)

class Plugin : public Object
{
	REFLECT_DECLARE_OBJECT(Plugin)

	friend class PluginManager;

public:

	Plugin();
	virtual ~Plugin();

	// Gets if plugin is enabled.
	bool isEnabled() const;

	// Gets metadata about this plugin.
	virtual PluginMetadata getMetadata() = 0;

	// Finds if there is a mode with given id.
	PluginTool* findTool( wxAuiToolBarItem* tool );

	// Plugin enable callback.
	virtual void onPluginEnable() = 0;
	virtual void onPluginDisable() = 0;

	// Plugin tool selection callback.
	virtual void onToolSelect( int id ) {}
	virtual void onToolUnselect( int id ) {}

	// Document creation callback.
	virtual void onDocumentCreate( Document& document ) {}
	virtual void onDocumentDestroy( Document& document ) {}

	// Document selection callback.
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

protected:

	// Helper method to disable plugins.
	void doPluginDisable();
	
	// Registers a new tool in the plugin.
	void addTool( const PluginTool& tool, bool addToMenu = false );
	void addTool( wxAuiToolBarItem* tool, bool addToMenu = false );

	// Removes all the registered tools.
	void removeTools();

	// Returns if the tool is from the plugin.
	bool hasTool(int id) const;

	// Removes a page from the main notebook.
	void removePage( wxWindow* page );

	// Is plugin enabled?
	bool enabled;

	// Keeps track of all the registered tools.
	PluginTools tools;

	// Editor frame.
	EditorFrame* editor;
};

//-----------------------------------//

} } // end namespaces