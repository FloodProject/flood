/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

namespace vapor { namespace editor {

class EditorFrame;
class Mode;

typedef std::map<int, Mode*> ModeIdMap;

//-----------------------------------//

/**
 * The editor can have different modes of interaction: editor camera 
 * interaction, object selection, object positioning (translating, 
 * rotating and scaling), terrain sculpting, terrain painting,
 * vegetation tools, river tools, road tools, etc.
 */

class Mode
{
public:

	Mode( EditorFrame* frame );
	virtual ~Mode();

	virtual void onModeInit( wxToolBar*, ModeIdMap& ) {}
	virtual void onModeEnter( int = -1 ) {}
	virtual void onModeExit() {}

	virtual void onNodeSelected() {}

	virtual void onMouseClick(short, short) {}
	virtual void onKeyPress() {}

protected:

	Engine* engine;
	EditorFrame* editor;
};

//-----------------------------------//

} } // end namespaces