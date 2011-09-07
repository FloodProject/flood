/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "EditorPlugin.h"
#include "Gizmo.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

class SelectionManager;
class SelectionOperation;
class GizmoOperation;

namespace GizmoTool
{
	enum Enum
	{
		Camera = wxID_HIGHEST + 837541,
		Translate,
		Rotate,
		Scale
	};
}

//-----------------------------------//

typedef std::map<EntityPtr, EntityPtr> GizmoMap;
typedef std::pair<const EntityPtr, EntityPtr> GizmoMapPair;

REFLECT_DECLARE_CLASS(GizmoPlugin)

class GizmoPlugin : public EditorPlugin
{
	REFLECT_DECLARE_OBJECT(GizmoPlugin)

public:

	GizmoPlugin();

	// Gets metadata about this plugin.
	PluginMetadata getMetadata() OVERRIDE;
	
	// Plugin callbacks.
	void onPluginEnable() OVERRIDE;
	void onPluginDisable() OVERRIDE;
	void reset();

	// Mouse events.
	void onMouseMove( const MouseMoveEvent& ) OVERRIDE;
	void onMouseDrag( const MouseDragEvent& ) OVERRIDE;
	void onMouseButtonPress( const MouseButtonEvent& ) OVERRIDE;
	void onMouseButtonRelease( const MouseButtonEvent& ) OVERRIDE;

	// Entity events.
	void onSceneUnload( const ScenePtr& ) OVERRIDE;
	void onEntitySelect( const EntityPtr& ) OVERRIDE;
	void onEntityUnselect( const EntityPtr& ) OVERRIDE;

	// Tool events.
	void onToolSelect( int id ) OVERRIDE;
	void onToolUnselect( int id ) OVERRIDE;

	// Selection events.
	void onSelection( SelectionOperation* );
	void onDeselection( SelectionOperation* );

protected:

	// Gets the selections manager.
	SelectionManager* getSelections();

	// Checks if the passed tool is the current one.
	bool isTool(GizmoTool::Enum mode);

	// Creates/removes a new gizmo for the given node.
	void createGizmo( const EntityPtr& node );
	void removeGizmo( const EntityPtr& node );

	// Creates a new gizmo undo/redo operation.
	void createOperation();

	// Performs bounding-box based ray picking.
	bool pickBoundingTest( const MouseMoveEvent& event );

	// Performs color based pixel picking.
	bool pickImageTest( const MouseMoveEvent& event, GizmoAxis::Enum& axis );

	// Gets the best plane for pick intersection.
	Plane getGizmoPickPlane();

	// Gets the point picked on the intersection plane.
	bool getGizmoPickPoint(int x, int y, Vector3& pickPoint);

	// Current Gizmo tool.
	GizmoTool::Enum tool;

	// Current Gizmo geometry.
	Gizmo* gizmo;

	// Current Gizmo axis selection.
	GizmoAxis::Enum axis;

	// Associates the object nodes to the gizmos nodes.
	GizmoMap gizmos;

	// Tracks if the gizmo is being picked.
	bool isGizmoPicked;

	// Current gizmo operation.
	GizmoOperation* op;

	// Current gizmo pick point.
	Vector3 firstPickPoint;

	// Toolbar buttons.
	wxAuiToolBarItem* buttonCamera;
	wxAuiToolBarItem* buttonTranslate;
	wxAuiToolBarItem* buttonRotate;
	wxAuiToolBarItem* buttonScale;
};

//-----------------------------------//

NAMESPACE_EDITOR_END