/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Mode.h"

namespace vapor { namespace editor {

//-----------------------------------//

class GizmoMode : public Mode
{
public:

	GizmoMode( EditorFrame* frame );
	//virtual ~GizmoMode();
	virtual void onModeInit(wxToolBar* toolbar, ModeIdMap& map);	
	virtual void onModeEnter( int id );
	virtual void onModeExit();

	virtual void onMouseClick(short x, short y);

protected:

	void disableSelectedNodes();

	// Nodes
	std::vector<NodePtr> selectedNodes;

	int currentTool;
};

//-----------------------------------//

} } // end namespaces