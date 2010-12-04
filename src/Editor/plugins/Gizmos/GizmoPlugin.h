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

namespace vapor { namespace editor {

//-----------------------------------//

class GizmoOperation;

//-----------------------------------//

namespace GizmoTool
{
	enum Enum
	{
		Camera = wxID_HIGHEST,
		Select,
		Translate,
		Rotate,
		Scale
	};
}

//-----------------------------------//

typedef std::map<EntityPtr, EntityPtr> GizmoMap;
typedef std::pair<const EntityPtr, EntityPtr> GizmoMapPair;

class GizmoPlugin : public Plugin
{
public:

	GizmoPlugin( EditorFrame* );

	// Gets metadata about this plugin.
	virtual PluginMetadata getMetadata();
	
	// Plugin enable callback.
	virtual void onPluginEnable();

	// Plugin disable callback.
	virtual void onPluginDisable();

	// Mouse events.
	virtual void onMouseMove( const MouseMoveEvent& );
	virtual void onMouseDrag( const MouseDragEvent& );
	virtual void onMouseButtonPress( const MouseButtonEvent& );
	virtual void onMouseButtonRelease( const MouseButtonEvent& );

	// Entity selection events.
	virtual void onEntitySelect( const EntityPtr& );
	virtual void onEntityUnselect( const EntityPtr& );

	// Scene load callback.
	void onSceneLoad( const ScenePtr& scene );

protected:

	// Handles gizmo tools buttons click.
	void onToolClick(wxCommandEvent& event);

	// Checks if the passed tool the current one.
	bool isTool(GizmoTool::Enum mode);

	// Unselects (and can also reselect) nodes (and their gizmos).
	void unselectEntities(bool reselect = false);

	// Creates a new gizmo for the given node.
	void createGizmo( const EntityPtr& node );

	// Removes the active gizmo for the given node.
	void removeGizmo( const EntityPtr& node );

	// Creates a new gizmo undo/redo operation.
	void createOperation();

	// Sets the bounding box visibility of the given node.
	void setBoundingBoxVisible( const EntityPtr& node, bool state );

	// Performs bounding-box based ray picking.
	bool pickBoundingTest( const MouseMoveEvent& event );

	// Performs color based pixel picking.
	bool pickImageTest( const MouseMoveEvent& event, GizmoAxis::Enum& axis );

	// Gets the best plane for pick intersection.
	Plane getGizmoPickPlane();

	// Gets the point picked on the intersection plane.
	bool getGizmoPickPoint(int x, int y, Vector3& pickPoint);

	// Gets the node picked by the ray.
	bool getGizmoPickEntity(int x, int y, EntityPtr& node);

	// Current Gizmo tool.
	GizmoTool::Enum tool;

	// Current Gizmo geometry.
	GizmoPtr gizmo;

	// Current Gizmo axis selection.
	GizmoAxis::Enum axis;

	// Associates the object nodes to the gizmos nodes.
	GizmoMap gizmos;

	// Current gizmo operation.
	GizmoOperation* op;

	// Current gizmo pick point.
	Vector3 firstPickPoint;

	// Editor scene.
	ScenePtr editorScene;

	// Toolbar buttons.
	wxToolBarToolBase* buttonCamera;
	wxToolBarToolBase* buttonSelect;
	wxToolBarToolBase* buttonTranslate;
	wxToolBarToolBase* buttonRotate;
	wxToolBarToolBase* buttonScale;
};

//-----------------------------------//

} } // end namespaces