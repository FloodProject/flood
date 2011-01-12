/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Gizmo.h"

namespace vapor { namespace editor {

//-----------------------------------//

class GizmoRotate : public Gizmo
{
public:

	GizmoRotate( const EntityPtr&, const CameraWeakPtr& );

	// Builds the gizmo geometry.
	virtual void buildGeometry();

protected:

	// Highlights an axis to provide visual feedback of handle selection.
	virtual void highlightAxis( GizmoAxis::Enum, bool highlight );

	// Generates the circles geometry.
	VertexBufferPtr generateCircles();

	// Generate circle geometry using sphere equation.
	void generateCircle(std::vector<Vector3>& pos, int slices);

	// Generate nice colors for the gizmo.
	void generateColors( std::vector<Vector3>& colors, const Color& c );

	// Lines geometry.
	VertexBufferPtr lines;
};

TYPEDEF_SHARED_POINTER_FROM_TYPE( GizmoRotate );

//-----------------------------------//

} } // end namespaces