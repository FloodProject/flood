/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Plugin.h"
#include "Gizmo.h"
#include "UndoOperation.h"

namespace vapor { namespace editor {

//-----------------------------------//

namespace GizmoTool
{
	enum Enum
	{
		Camera,
		Select,
		Translate,
		Rotate,
		Scale
	};
}

//-----------------------------------//

class GizmoOperation : public UndoOperation
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

class GizmoPlugin : public Plugin
{
public:

	GizmoPlugin( EditorFrame* );

	// Gets metadata about this plugin.
	virtual PluginMetadata getMetadata();
	
	// Plugin enable callback.
	virtual void onPluginEnable( wxToolBar* );

	// Plugin disable callback.
	virtual void onPluginDisable( wxToolBar* );

	// Mouse events.
	virtual void onMouseMove( const MouseMoveEvent& );
	virtual void onMouseDrag( const MouseDragEvent& );
	virtual void onMouseButtonPress( const MouseButtonEvent& );
	virtual void onMouseButtonRelease( const MouseButtonEvent& );

	// Node events.
	virtual void onNodeSelect( const NodePtr& );
	virtual void onNodeUnselect( const NodePtr& );

protected:

	// Called when a toolbar button is clicked.
	void onGizmoButtonClick(wxCommandEvent& event);

	bool isMode(GizmoTool::Enum mode) { return tool == mode; }

	void drawGizmo( NodePtr, NodePtr );
	void createGizmo( const NodePtr& );
	void removeGizmo( const NodePtr& );
	void createOperation();

	void setBoundingBoxVisible( const NodePtr& node, bool state );

	bool pickBoundingTest( const MouseMoveEvent& );
	bool pickImageTest( const MouseMoveEvent&, GizmoAxis::Enum& );
	
	void disableSelectedNodes();

	// Current Gizmo tool.
	GizmoTool::Enum tool;

	// Current Gizmo geometry.
	GizmoPtr gizmo;

	// Current Gizmo axis selection.
	GizmoAxis::Enum axis;

	// Current gizmo operation.
	GizmoOperation* op;
	
	ScenePtr editorScene;

	typedef std::map<NodePtr, NodePtr> GizmoNodeMap;
	GizmoNodeMap gizmos;
	std::vector<NodePtr> selected;

	// Toolbar buttons.
	wxToolBarToolBase* buttonCamera;
	wxToolBarToolBase* buttonSelect;
	wxToolBarToolBase* buttonTranslate;
	wxToolBarToolBase* buttonRotate;
	wxToolBarToolBase* buttonScale;
};

//-----------------------------------//

} } // end namespaces