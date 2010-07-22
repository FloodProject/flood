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

#include "vapor/render/Renderable.h"
#include "vapor/render/Material.h"

namespace vapor { namespace editor {

//-----------------------------------//

namespace GizmoAxis
{
	enum Enum
	{
		X = 0,
		Y = 2,
		Z = 4,
		None
	};
};

//-----------------------------------//

/**
 * Gizmos (also called manipulators) are used to manipulate the spatial
 * properties of scene objects. You can translate, rotate and scale in
 * a visual way using these. They have handles that you can pick and
 * drag with the mouse, so they are a pretty intuitive. This constructs
 * the vertices of the gizmos, which must be different for each tool to
 * easily distinguish between them. Translate gizmos are lines with a
 * cone shape in the end, like an arrow. Scale gizmos are similar but
 * instead of a cone, they have a cube. Rotate gizmos are the most
 * complex and have a set of circles in different orientations.
 */

class Gizmo : public Geometry
{
public:

	Gizmo( const NodePtr& node, const CameraWeakPtr& camera );

	// Selects an axis of the gizmo.
	void selectAxis( GizmoAxis::Enum );

	// Returns if any axis is currently selected in the gizmo.
	bool isAnyAxisSelected() const;

	// Deselects all axis of the gizmo.
	void deselectAxis();

	// Converts a color to a specific gizmo axis.
	static GizmoAxis::Enum getAxis(Color&);

	// Converts an axis enum to a unit vector.
	static Vector3 getUnitVector( GizmoAxis::Enum );

	// Gets the selected axis in the gizmo.
	GETTER(AxisSelected, GizmoAxis::Enum, selectedAxis)

	// Gets the node attached to the gizmo.
	GETTER(NodeAttachment, const NodePtr&, node)

	// Returns the name of this component.
	GETTER(Type, const std::string&, type)

	// Updates this component.
	virtual void update( double delta );

protected:

	// Generates the cone geometry.
	VertexBufferPtr generateCones();

	// Generates the lines geometry.
	VertexBufferPtr generateLines();

	// Generates solid cone geometry for the gizmo arrows.
	void generateSolidCone( double base, double height, uint slices,
		std::vector<Vector3>& pos );

	// Generate nice colors for the gizmo.
	void generateColors( uint slices, std::vector<Vector3>& colors,
		const Color& c1, const Color& c2 );

	NodePtr node;
	CameraWeakPtr weakCamera;

	VertexBufferPtr lines;
	VertexBufferPtr cones;

	GizmoAxis::Enum selectedAxis;

	static const std::string& type;
};

TYPEDEF_SHARED_POINTER_FROM_TYPE( Gizmo );

//-----------------------------------//

} } // end namespaces