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

	GizmoMode( EditorFrame* );
	
	virtual void onModeInit(wxToolBar*, ModeIdMap& );	
	virtual void onModeEnter( int );
	virtual void onModeExit();

	virtual void onMouseMove( const MouseMoveEvent& );
	virtual void onMouseDrag( const MouseDragEvent& );
	virtual void onMouseButtonPress( const MouseButtonEvent& );

	virtual void onNodeSelected( NodePtr, NodePtr );

protected:

	void drawGizmo( NodePtr, NodePtr );
	
	void enableBoundingGizmo( const NodePtr& );
	void disableBoundingGizmo( const NodePtr& );

	bool pickBoundingTest( const MouseMoveEvent& );
	bool pickImageTest( const MouseMoveEvent&, GizmoAxis::Enum& );
	
	void disableSelectedNodes();
	
	//FBOPtr fbo;
	//ViewportPtr view;
	//TexturePtr tex;

	int tool;
	ScenePtr editorScene;
	MouseMoveEvent oldMouseEvent;

	GizmoPtr gizmo;
	GizmoAxis::Enum axis;

	typedef std::map<NodePtr, NodePtr> GizmoNodeMap;
	GizmoNodeMap gizmos;
	std::vector<NodePtr> selected;
};

//-----------------------------------//

} } // end namespaces