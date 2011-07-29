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
class GizmoOperation;

namespace GizmoTool
{
	enum Enum
	{
		Camera = wxID_HIGHEST + 83,
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
	virtual PluginMetadata getMetadata() OVERRIDE;
	
	// Plugin enable callback.
	virtual void onPluginEnable() OVERRIDE;

	// Plugin disable callback.
	virtual void onPluginDisable();

	// Mouse events.
	virtual void onMouseMove( const MouseMoveEvent& ) OVERRIDE;
	virtual void onMouseDrag( const MouseDragEvent& ) OVERRIDE;
	virtual void onMouseButtonPress( const MouseButtonEvent& ) OVERRIDE;
	virtual void onMouseButtonRelease( const MouseButtonEvent& ) OVERRIDE;

	// Entity selection events.
	virtual void onEntitySelect( const EntityPtr& ) OVERRIDE;
	virtual void onEntityUnselect( const EntityPtr& ) OVERRIDE;

	// Tool selection callback.
	virtual void onToolSelect( int id ) OVERRIDE;

	// Tool unselection callback.
	void onToolUnselect( int id ) OVERRIDE;

protected:

	// Gets the selections manager.
	SelectionManager* getSelections();

	// Checks if the passed tool is the current one.
	bool isTool(GizmoTool::Enum mode);

	// Creates a new gizmo for the given node.
	void createGizmo( const EntityPtr& node );

	// Removes the active gizmo for the given node.
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
	GizmoPtr gizmo;

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