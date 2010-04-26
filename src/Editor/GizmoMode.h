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
#include "Operation.h"

namespace vapor { namespace editor {

//-----------------------------------//

namespace GizmoTool
{
	enum Enum
	{
		Camera = 18237,
		Select,
		Translate,
		Rotate,
		Scale
	};
}

//-----------------------------------//

class GizmoOperation : public Operation
{
public:

	GizmoOperation();
	
	void undo();
	void redo();
	void process( bool undo );

	NodeWeakPtr weakNode;
	GizmoTool::Enum tool;
	GizmoAxis::Enum axis;
	GizmoPtr gizmo;

	Vector3 orig_translation;
	Vector3 orig_scale;
	EulerAngles orig_rotation;

	Vector3 translation;
	Vector3 scale;
	EulerAngles rotation;
};

//-----------------------------------//

class GizmoMode : public Mode
{
public:

	GizmoMode( EditorFrame* );
	
	virtual void onModeInit( wxToolBar*, ModeIdMap& );	
	virtual void onModeEnter( int );
	virtual void onModeExit();

	virtual void onMouseMove( const MouseMoveEvent& );
	virtual void onMouseDrag( const MouseDragEvent& );
	virtual void onMouseButtonPress( const MouseButtonEvent& );
	virtual void onMouseButtonRelease( const MouseButtonEvent& );

	virtual void onNodeSelected( NodePtr, NodePtr );

protected:

	void createOperation();
	void drawGizmo( NodePtr, NodePtr );
	
	void enableBoundingGizmo( const NodePtr& );
	void disableBoundingGizmo( const NodePtr& );

	bool pickBoundingTest( const MouseMoveEvent& );
	bool pickImageTest( const MouseMoveEvent&, GizmoAxis::Enum& );
	
	void disableSelectedNodes();

	GizmoTool::Enum tool;
	ScenePtr editorScene;

	GizmoPtr gizmo;
	GizmoAxis::Enum axis;

	typedef std::map<NodePtr, NodePtr> GizmoNodeMap;
	GizmoNodeMap gizmos;
	std::vector<NodePtr> selected;

	// Holds the current gizmo operation.
	GizmoOperation* op;
};

//-----------------------------------//

} } // end namespaces