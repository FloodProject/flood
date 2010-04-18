/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Mode.h"
#include "Gizmo.h"

namespace vapor { namespace editor {

//-----------------------------------//

class GizmoMode : public Mode
{
public:

	GizmoMode( EditorFrame* frame );
	
	virtual void onModeInit(wxToolBar* toolbar, ModeIdMap& map);	
	virtual void onModeEnter( int id );
	virtual void onModeExit();

	virtual void onNodeSelected( NodePtr old, NodePtr new_ );

	virtual void onMouseMove( const MouseMoveEvent& me );
	virtual void onMouseButtonPress( const MouseButtonEvent& mbe );

protected:

	void drawGizmo( NodePtr old, NodePtr new_ );
	
	void enableBoundingGizmo( const NodePtr& node );
	void disableBoundingGizmo( const NodePtr& node );

	void disableSelectedNodes();

	ScenePtr editorScene;
	GizmoPtr gizmo;

	std::vector<NodePtr> selectedNodes;

	typedef std::map<NodePtr, NodePtr> GizmoNodeMap;
	GizmoNodeMap gizmos;

	int currentTool;
};

//-----------------------------------//

} } // end namespaces