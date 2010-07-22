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

class Tool;
typedef std::map<int, Tool*> ToolsMap;

//-----------------------------------//

/**
 * A 3D editor needs to provide different kind of tools to the user,
 * so to be flexible and allow extensibility each tool behaviours and
 * functionality is provided by a custom type derived from this class.
 * As each tool can be loaded/unloaded at runtime, there is a tool
 * lifecycle you must implement. Most of the work is done by asking
 * the editor to execute actions in the different events/callbacks.
 */

class Tool
{
public:

	Tool( EditorFrame* frame );
	virtual ~Tool();

	// Tool initialization callback.
	virtual void onToolInit( wxToolBar*, ToolsMap& ) {}

	// Tool deinitialization callback.
	virtual void onToolDeinit( wxToolBar*, ToolsMap& ) {}

	// Tool enable callback.
	virtual void onToolEnable( int tool = -1 ) {}
	
	// Tool disable callback.
	virtual void onToolDisable() {}

	// Node selection callback.
	virtual void onNodeSelected( NodePtr, NodePtr ) {}

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

	// Access to the engine instance.
	Engine* engine;

	// Access to the editor instance.
	EditorFrame* editor;

	// Access to the main viewframe.
	Viewframe* viewframe;
};

//-----------------------------------//

} } // end namespaces