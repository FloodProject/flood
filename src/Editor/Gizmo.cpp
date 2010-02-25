/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "PCH.h"
#include "Gizmo.h"

namespace vapor { namespace editor {

using namespace vapor::math;

//-----------------------------------//

const std::string& Gizmo::type = "Gizmo";

//-----------------------------------//

Gizmo::Gizmo( const math::Vector3 midPoint )
{
	RenderablePtr rend( new Renderable(
		Primitive::Lines, getVB( midPoint ) ) );
	
	MaterialPtr mat( new Material( "Gizmo" ) );
	mat->setLineWidth( 5.0f );
	mat->setBackfaceCulling( false );
	mat->setProgram( "diffuse" );
	rend->setMaterial( mat );
	
	addRenderable( rend );
}

//-----------------------------------//

render::VertexBufferPtr Gizmo::getVB( const math::Vector3& midPoint )
{
	static float S = 100.0;

	// Create a new VBO and upload triangle data
	VertexBufferPtr vb( new VertexBuffer() );

	// Vertex position data
	std::vector< Vector3 > pos;

	// Vertex color data
	std::vector< Vector3 > colors;

	// X axis (red)
	pos.push_back( midPoint + Vector3::Zero );
	pos.push_back( midPoint + Vector3::UnitX*S );
	colors.push_back( Colors::Red );
	colors.push_back( Colors::Red );

	// Y axis (green)
	pos.push_back( midPoint + Vector3::Zero );
	pos.push_back( midPoint + Vector3::UnitY*S );
	colors.push_back( Colors::Green );
	colors.push_back( Colors::Green );

	// Z axis (blue)
	pos.push_back( midPoint + Vector3::Zero );
	pos.push_back( midPoint + Vector3::UnitZ*S );
	colors.push_back( Colors::Blue );
	colors.push_back( Colors::Blue );

	// Vertex buffer setup
	vb->set( VertexAttribute::Position, pos );
	vb->set( VertexAttribute::Color, colors );

	return vb;
}

//-----------------------------------//

const std::string& Gizmo::getType() const
{
	return type;
}

//-----------------------------------//

} } // end namespaces