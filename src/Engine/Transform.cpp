/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/scene/Transform.h"
#include "vapor/scene/Node.h"
#include "vapor/math/Math.h"

using namespace vapor::math;
using namespace vapor::render;

namespace vapor { namespace scene {

//-----------------------------------//

const std::string& Transform::type = "Transform";

//-----------------------------------//

Transform::Transform( float x, float y, float z )
	: v_scale( 1.0f ), isPhysicsDriven( false ), v_translate( x, y, z ),
	aabbNeedsUpdate( true )
{

}

//-----------------------------------//

Transform::~Transform()
{

}

//-----------------------------------//

void Transform::translate( const math::Vector3& tr )
{
	translate( tr.x, tr.y, tr.z );
}

//-----------------------------------//

void Transform::translate( float x, float y, float z )
{
	v_translate.x += x;
	v_translate.y += y;
	v_translate.z += z;

	notify();
}

//-----------------------------------//

void Transform::scale( float uniform )
{
	scale( uniform, uniform, uniform );
}

//-----------------------------------//

void Transform::scale( const math::Vector3& s )
{
	scale( s.x, s.y, s.z );
}

//-----------------------------------//

void Transform::scale( float x, float y, float z )
{
	v_scale.x *= x;
	v_scale.y *= y;
	v_scale.z *= z;

	notify();
}

//-----------------------------------//

void Transform::rotate( const math::Vector3& rot )
{
	rotate( rot.x, rot.y, rot.z );
}

//-----------------------------------//

void Transform::rotate( float xang, float yang, float zang )
{
	angles.xang += xang;
	angles.yang += yang;
	angles.zang += zang;

	notify();
}

//-----------------------------------//

const math::EulerAngles& Transform::getRotation() const
{
	return angles;
}

//-----------------------------------//

void Transform::setRotation( math::EulerAngles& rot )
{
	this->angles = rot;

	notify();
}

//-----------------------------------//

const math::Vector3& Transform::getPosition() const
{
	return v_translate;
}

//-----------------------------------//

void Transform::setPosition( const math::Vector3& position )
{
	v_translate = position;
}

//-----------------------------------//

math::Matrix4x3 Transform::lookAt( const math::Vector3& lookAtVector, const math::Vector3& upVector )
{
	const Vector3& eye = v_translate;

	Vector3 zaxis = ( eye - lookAtVector ).normalize();
	Vector3	xaxis = upVector.cross(zaxis).normalize();
	Vector3	yaxis = zaxis.cross(xaxis);

	Matrix4x3 m;
	m.m11 = xaxis.x;
	m.m12 = yaxis.x;
	m.m13 = zaxis.x;

	m.m21 = xaxis.y;
	m.m22 = yaxis.y;
	m.m23 = zaxis.y;

	m.m31 = xaxis.z;
	m.m32 = yaxis.z;
	m.m33 = zaxis.z;

	m.tx = -xaxis.dot(eye);
	m.ty = -yaxis.dot(eye);
	m.tz = -zaxis.dot(eye);
	
	return m;
}

//-----------------------------------//

void Transform::reset( )
{
	v_translate.zero();
	v_scale = math::Vector3( 1.0f );
	angles.identity();
	
	notify();
}

//-----------------------------------//

void Transform::setAbsoluteTransform( const math::Matrix4x3& matrix )
{
	absoluteLocalToWorld = matrix;
}

//-----------------------------------//

const math::Matrix4x3& Transform::getAbsoluteTransform() const
{
	return absoluteLocalToWorld;
}

//-----------------------------------//

math::Matrix4x3 Transform::getLocalTransform() const
{
	return Matrix4x3::createTranslationMatrix( v_translate )
		* angles.getOrientationMatrix()
		* Matrix4x3::createScaleMatrix( v_scale );
}

//-----------------------------------//

const std::string& Transform::getType() const
{
	return Transform::type;
}

//-----------------------------------//

bool Transform::requiresBoundingVolumeUpdate() const
{
	// TODO: optimize this
	return aabbNeedsUpdate;
}

//-----------------------------------//

static const float EXTRA_SPACE = 1.01f;

#define ADD_BOX_FACE( a, b, c, d )						\
	v.push_back( aabb.getCorner( a ) * EXTRA_SPACE );	\
	v.push_back( aabb.getCorner( b ) * EXTRA_SPACE );	\
	v.push_back( aabb.getCorner( c ) * EXTRA_SPACE );	\
	v.push_back( aabb.getCorner( d ) * EXTRA_SPACE );

RenderablePtr buildBoundingRenderable( const math::AABB& aabb )
{
	VertexBufferPtr vb( new VertexBuffer() );

	std::vector< Vector3 > v;
	
	ADD_BOX_FACE( 0, 2, 3, 1 ) // Front
	ADD_BOX_FACE( 0, 1, 5, 4 ) // Bottom
	ADD_BOX_FACE( 4, 5, 7, 6 ) // Back
	ADD_BOX_FACE( 2, 6, 7, 3 ) // Top
	ADD_BOX_FACE( 0, 4, 6, 2 ) // Left
	ADD_BOX_FACE( 1, 3, 7, 5 ) // Right

	std::vector< Vector3 > c( 6/*faces*/*4/*vertices*/, Vector3( 1.0f, 1.0f, 0.0f ) );

	vb->set( VertexAttribute::Vertex, v );
	vb->set( VertexAttribute::Color, c );

	MaterialPtr mat( new Material( "BoundBox", "diffuse" ) );
	mat->setLineWidth( 1.0f );
	mat->setLineSmoothing( true );

	RenderablePtr bbox( new Renderable( Primitive::Quads, vb, mat ) );
	bbox->setPolygonMode( PolygonMode::Wireframe );

	return bbox;
}

//-----------------------------------//

void Transform::update( double delta )
{
	absoluteLocalToWorld = getLocalTransform();

	if( !requiresBoundingVolumeUpdate() )
		return;

	assert( getNode() );
	NodePtr node = getNode();

	boundingVolume.reset();

	foreach( const GeometryPtr& geometry, node->getGeometry() )
	{
		boundingVolume.add( geometry->getBoundingVolume() );
	}
	
	aabbNeedsUpdate = false;

	// Update debug renderable
	aabbRenderable = buildBoundingRenderable( boundingVolume );
}

//-----------------------------------//

void Transform::notify()
{
	if( onTransform.empty() ) 
	{
		return;
	}

	onTransform();
}

//-----------------------------------//

const math::AABB& Transform::getBoundingVolume() const
{
	return boundingVolume;
}

//-----------------------------------//

const math::AABB& Transform::getWorldBoundingVolume() const
{
	return worldBoundingVolume;
}

//-----------------------------------//

render::RenderablePtr Transform::getDebugRenderable() const
{
	return aabbRenderable;
}

//-----------------------------------//

} } // end namespaces