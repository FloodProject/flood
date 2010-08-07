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

class Viewframe;
class EditorFrame;
class PluginManager;

//-----------------------------------//

struct PluginMetadata
{
	// Name of this plugin.
	std::string name;

	// Description of this plugin.
	std::string description;

	// Author of this plugin.
	std::string author;

	// Version of this plugin.
	std::string version;
};

//-----------------------------------//

/**
 * A 3D editor needs to provide different kind of tools to the user,
 * so to be flexible and allow extensibility each tool behaviours and
 * functionality is provided by a custom type derived from this class.
 * As each tool can be loaded/unloaded at runtime, there is a tool
 * lifecycle you must implement. Most of the work is done by asking
 * the editor to execute actions in the different events/callbacks.
 */

class Plugin
{
	friend class PluginManager;

public:

	Plugin( EditorFrame* frame );
	virtual ~Plugin();

	// Gets if plugin is enabled.
	bool isPluginEnabled() const;

	// Gets metadata about this plugin.
	virtual PluginMetadata getMetadata() = 0;

	// Plugin enable callback.
	virtual void onPluginEnable() = 0;

	// Plugin disable callback.
	virtual void onPluginDisable() = 0;

	// Node selection callback.
	virtual void onNodeSelect( const NodePtr& ) {}
	
	// Node unselection callback.
	virtual void onNodeUnselect( const NodePtr& ) {}

	// Mouse move callback.
	virtual void onMouseMove( const MouseMoveEvent& ) {}
	
	// Mouse drag callback.
	virtual void onMouseDrag( const MouseDragEvent& ) {}
	
	// Mouse button press callback.
	virtual void onMouseButtonPress( const MouseButtonEvent& ) {}
	
	// Mouse button release callback.
	virtual void onMouseButtonRelease( const MouseButtonEvent& ) {}
	
	// Mouse enter callback.
	virtual void onMouseEnter() {}
	
	// Mouse leave callback.
	virtual void onMouseLeave() {}
	
	// Key press callback.
	virtual void onKeyPress() {}

protected:

	// Helper method to disable plugins.
	void doPluginDisable();

	// Registers a new tool in the plugin.
	void addTool( wxToolBarToolBase* tool );

	// Removes all the registered tools.
	void removeTools();

	// Removes a page from the main notebook.
	void removePage( wxWindow* page );

	// Access to the engine instance.
	Engine* engine;

	// Access to the editor instance.
	EditorFrame* editor;

	// Access to the main viewframe.
	Viewframe* viewframe;

	// Is plugin enabled?
	bool pluginEnabled;

	// Keeps track of all the registered tools.
	typedef std::vector<wxToolBarToolBase*> Tools;
	Tools tools;
};

//-----------------------------------//

} } // end namespaces