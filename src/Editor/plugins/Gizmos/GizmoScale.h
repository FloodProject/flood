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

class GizmoScale : public Gizmo
{
public:

	GizmoScale( const EntityPtr&, const CameraWeakPtr& );

	// Builds the gizmo geometry.
	virtual void buildGeometry();

protected:

	// Highlights an axis to provide visual feedback of handle selection.
	virtual void highlightAxis( GizmoAxis::Enum, bool highlight );

	// Generates the cubes geometry.
	VertexBufferPtr generateCubes();

	// Generate nice colors for the gizmo.
	void generateColors( std::vector<Vector3>& colors, const Color& c );

	// Lines geometry.
	VertexBufferPtr lines;

	// Cubes geometry.
	VertexBufferPtr cubes;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( GizmoScale );

//-----------------------------------//

NAMESPACE_EDITOR_END