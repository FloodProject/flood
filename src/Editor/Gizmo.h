/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/scene/Geometry.h"
#include "vapor/render/Renderable.h"
#include "vapor/render/Material.h"

namespace vapor { namespace editor {

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

class VAPOR_API Gizmo : public scene::Geometry
{
public:

	Gizmo( const math::Vector3 midPoint = math::Vector3::Zero );

	// Returns the name of this component.
	virtual const std::string& getType() const;

protected:

	// Generates all the cone geometries in a single vertex buffer.
	render::RenderablePtr generateCones();

	// Generates solid cone geometry for the gizmo arrows.
	void generateSolidCone( double base, double height, uint slices,
		std::vector<Vector3>& pos );
	
	render::VertexBufferPtr generateLines();

	math::Vector3 midPoint;

	static const std::string& type;
};

//-----------------------------------//

TYPEDEF_SHARED_POINTER_FROM_CLASS( Gizmo );

//-----------------------------------//

} } // end namespaces