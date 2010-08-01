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

class GizmoTranslate : public Gizmo
{
public:

	GizmoTranslate( const NodePtr& node, const CameraWeakPtr& camera );

	// Builds the gizmo geometry.
	virtual void buildGeometry();

	// Converts a color to a specific gizmo axis.
	virtual GizmoAxis::Enum getAxis(Color&);

	// Returns the name of this component.
	//GETTER(Type, const std::string&, type)

protected:

	// Highlights an axis to provide visual feedback of handle selection.
	virtual void highlightAxis( GizmoAxis::Enum, bool highlight );

	// Generates the cone geometry.
	VertexBufferPtr generateCones();

	// Generates solid cone geometry for the gizmo arrows.
	void generateSolidCone( double base, double height, uint slices,
		std::vector<Vector3>& pos );

	// Generate nice colors for the gizmo.
	void generateColors( std::vector<Vector3>& colors, const Color& c );

	// Lines geometry.
	VertexBufferPtr lines;

	// Cones geometry.
	VertexBufferPtr cones;

	static const std::string& type;
};

TYPEDEF_SHARED_POINTER_FROM_TYPE( GizmoTranslate );

//-----------------------------------//

} } // end namespaces