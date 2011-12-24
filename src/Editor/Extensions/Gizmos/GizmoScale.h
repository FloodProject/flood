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
	void buildGeometry() OVERRIDE;

protected:

	// Highlights an axis to provide visual feedback of handle selection.
	void highlightAxis( GizmoAxis::Enum, bool highlight ) OVERRIDE;

	// Generates the cubes geometry.
	GeometryBufferPtr generateCubes();

	// Generate nice colors for the gizmo.
	void generateColors( std::vector<Vector3>& colors, const Color& c );

	// Lines geometry.
	GeometryBufferPtr lines;

	// Cubes geometry.
	GeometryBufferPtr cubes;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( GizmoScale );

//-----------------------------------//

NAMESPACE_EDITOR_END