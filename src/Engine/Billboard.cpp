/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/scene/Billboard.h"
#include "vapor/render/Material.h"
#include "vapor/scene/Node.h"

namespace vapor {

//-----------------------------------//

BEGIN_CLASS_PARENT(Billboard, Component)
END_CLASS()

//-----------------------------------//

Billboard::Billboard( const CameraPtr& cam, BillboardType::Enum type )
	: camera(cam)
	, billboardType(type)
	, rend( new Renderable(PolygonPrimitive::Lines) )
{ }

//-----------------------------------//

void Billboard::update( double VAPOR_UNUSED(delta) )
{
	// If this is the first update loop, we need to get the transform.
	if( !transform )
		transform = getNode()->getTransform();

	// If we have no components, nothing to do...
	if( !camera || !transform )
		return;
	
	const Matrix4x3& view = camera->getViewMatrix();

	//Vector3 right( view.m11, view.m12, view.m13 );
	//right.normalize();
	
	Vector3 up( view.m21, view.m22, view.m23 );
	up.normalize();

	Vector3 forward( view.m31, view.m32, view.m33 );
	forward.normalize();

	Matrix4x3 lookAt = transform->lookAt( -forward, up );
	
	//Vector3 position = transform->getPosition();
	//lookAt.tx = position.x;
	//lookAt.ty = position.y;
	//lookAt.tz = position.z;
	
	transform->setAbsoluteTransform( lookAt );
}

//-----------------------------------//

RenderablePtr Billboard::getDebugRenderable() const
{
	if( rend->getVertexBuffer() )
		return rend;

	VertexBufferPtr vb( new VertexBuffer() );
	
	std::vector<Vector3> pos;
	std::vector<Vector3> colors;

	TransformPtr cameraTransform = camera->getNode()->getTransform();
	Vector3 lookAt = transform->getPosition()-cameraTransform->getPosition();
	
	pos.push_back( transform->getPosition() );
	pos.push_back( cameraTransform->getPosition() );

	colors.push_back( Vector3::Zero );
	colors.push_back( Vector3::Zero );
	
	Vector3 up = camera->getLookAtVector().cross( Vector3::UnitY );

	pos.push_back( up );
	pos.push_back( up*3 );

	colors.push_back( Vector3::Zero );
	colors.push_back( Vector3::Zero );

	vb->set( VertexAttribute::Position, pos );
	vb->set( VertexAttribute::Color, colors );
	
	MaterialPtr mat( new Material("Billboard") );

	rend->setVertexBuffer( vb );
	rend->setMaterial( mat );

	return rend;
}

//-----------------------------------//

} // end namespace