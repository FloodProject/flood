/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/scene/Geometry.h"
#include "vapor/scene/Camera.h"

namespace vapor { namespace editor {

//-----------------------------------//

namespace GizmoAxis
{
	enum Enum
	{
		X = 0,
		Y,
		Z,
		None
	};
};

//-----------------------------------//

/**
 * Gizmos (also called 3D manipulators) are used to manipulate the
 * spatial properties of scene objects. Tthey allow to translate,
 * rotate and scale objects in an interactive and intuitive way.
 * Gizmos provide a set of handles you pick and drag with the mouse.
 * Translate gizmos are made by lines with a cone shape in the end.
 * Scale gizmos are similar but instead of a cone, they have a cube.
 * Rotate gizmos have a set of circles in different orientations.
 */

class Gizmo : public Geometry
{
public:

	Gizmo( const NodePtr& node, const CameraWeakPtr& camera );

	// Builds the gizmo geometry.
	virtual void buildGeometry() = 0;

	// Selects (highlights) an axis of the gizmo.
	void selectAxis( GizmoAxis::Enum );

	// Deselects all axis of the gizmo.
	void deselectAxis();

	// Returns if any axis is currently selected in the gizmo.
	bool isAnyAxisSelected() const;

	// Converts a color to a specific gizmo axis.
	virtual GizmoAxis::Enum getAxis(Color&);

	// Converts an axis enum to a unit vector.
	static Vector3 getAxisVector( GizmoAxis::Enum );

	// Updates the position and scale of the gizmo.
	void updatePositionScale(); 

	// Gets the selected axis in the gizmo.
	GETTER(AxisSelected, GizmoAxis::Enum, selectedAxis)

	// Gets the node attached to the gizmo.
	GETTER(NodeAttachment, const NodePtr&, nodeObject)

	// Updates this component.
	virtual void update( double delta );

	// Returns the type of this component.
	GETTER(Type, const std::string&, type)

protected:

	// Highlights an axis to provide visual feedback of handle selection.
	virtual void highlightAxis( GizmoAxis::Enum, bool highlight ) = 0;

	// Generates the lines geometry.
	VertexBufferPtr generateLines();
	void generateLinesColors(std::vector<Vector3>& colors);

	NodePtr nodeObject;
	MaterialPtr material;
	CameraWeakPtr weakCamera;
	GizmoAxis::Enum selectedAxis;

	// Gizmo colors.
	static const Color& X;
	static const Color& Y;
	static const Color& Z;

	static const std::string& type;
};

TYPEDEF_SHARED_POINTER_FROM_TYPE( Gizmo );

//-----------------------------------//

} } // end namespaces