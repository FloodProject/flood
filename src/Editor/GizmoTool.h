/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Tool.h"
#include "Gizmo.h"
#include "Operation.h"

namespace vapor { namespace editor {

//-----------------------------------//

namespace GizmoType
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
	GizmoType::Enum tool;
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

class GizmoTool : public Tool
{
public:

	GizmoTool( EditorFrame* );
	
	virtual void onToolInit( wxToolBar*, ToolsMap& );	
	virtual void onToolEnable( int );
	virtual void onToolDisable();

	virtual void onMouseMove( const MouseMoveEvent& );
	virtual void onMouseDrag( const MouseDragEvent& );
	virtual void onMouseButtonPress( const MouseButtonEvent& );
	virtual void onMouseButtonRelease( const MouseButtonEvent& );

	virtual void onNodeSelected( NodePtr, NodePtr );

protected:

	bool isMode(GizmoType::Enum mode) { return tool == mode; }

	void createOperation();
	void drawGizmo( NodePtr, NodePtr );
	
	void createGizmo( const NodePtr& );
	void removeGizmo( const NodePtr& );
	void setBoundingBoxVisible( const NodePtr& node, bool state );

	bool pickBoundingTest( const MouseMoveEvent& );
	bool pickImageTest( const MouseMoveEvent&, GizmoAxis::Enum& );
	
	void disableSelectedNodes();

	GizmoType::Enum tool;
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