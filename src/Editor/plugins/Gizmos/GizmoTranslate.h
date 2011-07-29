/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Gizmo.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

class GizmoTranslate : public Gizmo
{
public:

	GizmoTranslate( const EntityPtr&, const CameraWeakPtr& );

	// Builds the gizmo geometry.
	virtual void buildGeometry();

	// Converts a color to a specific gizmo axis.
	virtual GizmoAxis::Enum getAxis(Color&);

protected:

	// Highlights an axis to provide visual feedback of handle selection.
	virtual void highlightAxis( GizmoAxis::Enum, bool highlight );

	// Generates the cone geometry.
	VertexBufferPtr generateCones();

	// Generates solid cone geometry for the gizmo arrows.
	void generateSolidCone( float base, float height, uint slices,
		std::vector<Vector3>& pos );

	// Generate nice colors for the gizmo.
	void generateColors( std::vector<Vector3>& colors, const Color& c );

	// Lines geometry.
	VertexBufferPtr lines;

	// Cones geometry.
	VertexBufferPtr cones;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( GizmoTranslate );

//-----------------------------------//

NAMESPACE_EDITOR_END