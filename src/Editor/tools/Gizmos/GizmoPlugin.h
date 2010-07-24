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
		Camera = 18237,
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
	
	virtual void onPluginEnable( wxToolBar*, PluginsMap& );
	virtual void onPluginDisable( wxToolBar*, PluginsMap& );

	virtual void onToolEnable( int );
	virtual void onToolDisable();

	virtual void onMouseMove( const MouseMoveEvent& );
	virtual void onMouseDrag( const MouseDragEvent& );
	virtual void onMouseButtonPress( const MouseButtonEvent& );
	virtual void onMouseButtonRelease( const MouseButtonEvent& );

	virtual void onNodeSelect( const NodePtr& );
	virtual void onNodeUnselect( const NodePtr& );

protected:

	bool isMode(GizmoTool::Enum mode) { return tool == mode; }

	void createOperation();
	void drawGizmo( NodePtr, NodePtr );
	
	void createGizmo( const NodePtr& );
	void removeGizmo( const NodePtr& );
	void setBoundingBoxVisible( const NodePtr& node, bool state );

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

	std::vector<wxToolBarToolBase*> tools;

	// Holds the current gizmo operation.
	GizmoOperation* op;
};

//-----------------------------------//

} } // end namespaces