/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/scene/Camera.h"
#include "vapor/scene/Group.h"
#include "vapor/scene/Geometry.h"
#include "vapor/math/Math.h"

namespace vapor { namespace scene {

using namespace vapor::math;
using namespace vapor::render;

const std::string& Camera::type = "Camera";

//-----------------------------------//

Camera::Camera( render::Device* device, Projection::Enum proj )
	: renderDevice( device ), projection( proj ), fov(60.0f),
	near_(5.0f), far_(1000.0f), lookAtVector(Vector3::UnitZ),
	viewport(nullptr), viewSize(Vector2i::Zero)
{
	assert( device != nullptr );
}

//-----------------------------------//

Camera::Camera( const Camera& rhs )
	: renderDevice( rhs.renderDevice ), projection( rhs.projection ),
	fov( rhs.fov ), near_( rhs.near_ ), far_( rhs.far_ )
{
}

//-----------------------------------//

Camera::~Camera()
{
	if( transform )
	{
		transform->onTransform -=
			fd::bind( &Camera::onTransform, this );
	}
}

//-----------------------------------//

math::Ray Camera::getRay( float screenX, float screenY, math::Vector3* outFar ) const
{
	assert( viewport != nullptr );

	Matrix4x4 view4( viewMatrix );
	Matrix4x4 inverseVP = (view4*projectionMatrix).inverse();
	
	Vector2i size = viewport->getSize();

	//screenX = screenX / size.x;
	//screenY = screenY / size.y;

	float nx = (2.0f * screenX) - 1.0f;
	float ny = 1.0f - (2.0f * screenY);

	Vector3 nearPoint(nx, ny, -1.f);
	// Use midPoint rather than far point
	Vector3 midPoint (nx, ny,  0.0f);

	// Get ray origin and ray target on near plane in world space
	Vector3 rayOrigin, rayTarget;
	
	rayOrigin = inverseVP * nearPoint;
	rayTarget = inverseVP * midPoint;

	Vector3 rayDirection = rayTarget - rayOrigin;
	rayDirection.normalize();

	//Ray pickRay(rayOrigin, rayDirection);

	if( outFar )
		*outFar = midPoint;

	double m[16];
	m[0] = viewMatrix.m11;
	m[1] = viewMatrix.m21;
	m[2] = viewMatrix.m31;
	m[3] = viewMatrix.tx;

	m[4] = viewMatrix.m12;
	m[5] = viewMatrix.m22;
	m[6] = viewMatrix.m32;
	m[7] = viewMatrix.ty;

	m[8] = viewMatrix.m13;
	m[9] = viewMatrix.m23;
	m[10] = viewMatrix.m33;
	m[11] = viewMatrix.tz;

	m[12] = 0;
	m[13] = 0;
	m[14] = 1;
	m[15] = 0;

	double p[16];
	p[0] = projectionMatrix.m11;
	p[1] = projectionMatrix.m21;
	p[2] = projectionMatrix.m31;
	p[3] = projectionMatrix.tx;

	p[4] = projectionMatrix.m12;
	p[5] = projectionMatrix.m22;
	p[6] = projectionMatrix.m32;
	p[7] = projectionMatrix.ty;

	p[8] = projectionMatrix.m13;
	p[9] = projectionMatrix.m23;
	p[10] = projectionMatrix.m33;
	p[11] = projectionMatrix.tz;

	p[12] = projectionMatrix.m14;
	p[13] = projectionMatrix.m24;
	p[14] = projectionMatrix.m34;
	p[15] = projectionMatrix.tw;

	//int v[] = { 0, 0, size.x, size.y };

	Vector3d n, f;
	//gluUnProject(screenX, screenY, 0.0, m, p, v, &n.x, &n.y, &n.z );
	//gluUnProject(screenX, screenY, 1.0, m, p, v, &f.x, &f.y, &f.z );

	Vector3d d = f - n;
	d.normalize();

	Vector3 nf;
	nf.x = (float) n.x;
	nf.y = (float) n.y;
	nf.z = (float) n.z;

	Vector3 ff;
	ff.x = (float) f.x;
	ff.y = (float) f.y;
	ff.z = (float) f.z;

	Vector3 df;
	df.x = (float) d.x;
	df.y = (float) d.y;
	df.z = (float) d.z;

	Ray pickRay(nf, df);

	if( outFar )
		*outFar = ff;

	return pickRay;
}

//-----------------------------------//

void Camera::update( double VAPOR_UNUSED(delta) )
{
	// Only run the following code once.
	if( transform ) return;

	assert( getNode() != nullptr );
	assert( getNode()->getTransform() != nullptr );
		
	transform = getNode()->getTransform();
	transform->onTransform += fd::bind( &Camera::onTransform, this );

	// Update the view matrix the first update.
	/*setupProjection();*/
	setupView();
}

//-----------------------------------//

void Camera::setupProjection( const Vector2i& size )
{
	if( projection == Projection::Perspective )
	{
		projectionMatrix = Matrix4x4::createPerspectiveProjection( 
			fov, viewport->getAspectRatio(), near_, far_ );
	}
	else
	{
		projectionMatrix = Matrix4x4::createOrthographicProjection( 
			0.0f, (float)size.x, 0.0f, (float)size.y, near_, far_ );
	}
}

//-----------------------------------//

void Camera::setupView()
{
	const Vector3& position = transform->getPosition();
	const EulerAngles& rotation = transform->getRotation();
	
	// Update the lookAt vector.
	Vector3 forward = Vector3::UnitZ * rotation.getOrientationMatrix();
	lookAtVector = position + forward;
	
	// Update the view matrix.
	viewMatrix = transform->lookAt( lookAtVector, Vector3::UnitY );
}

//-----------------------------------//

void Camera::setViewport( ViewportPtr newViewport )
{
	assert( newViewport != nullptr );

	viewport = newViewport;
	Vector2i newSize = viewport->getSize();

	if( viewSize != newSize )
	{
		viewSize = newSize;
		setupProjection( viewSize );
		renderDevice->setViewport( Vector2i::Zero, viewSize );
	}
}

//-----------------------------------//

void Camera::onTransform()
{
	assert( transform != nullptr );
	setupView();
}

//-----------------------------------//

void Camera::render( const NodePtr& node, bool clearView ) const
{
	if( !viewport ) return;

	// This will contain all nodes used for rendering.
	render::RenderBlock renderBlock;

	cull( renderBlock, node );

	if( clearView )
	{
		renderDevice->setClearColor( viewport->getClearColor() );
		renderDevice->clearTarget();
	}

	renderDevice->render( renderBlock, this );
}

//-----------------------------------//

void Camera::render() const
{
	NodePtr node = getNode();
	assert( node != nullptr );

	// Search for the root node.
	while ( node->getParent() )
	{
		node = node->getParent();
	}
	  
	render( node );
}

//-----------------------------------//

void Camera::cull( render::RenderBlock& block, const NodePtr& node ) const
{
	// Let's forget culling for now. Return all renderable nodes.
	// TODO: Check if dynamic_cast is faster than a string comparison.
	
	// Try to see if this is a Group-derived node.
	const GroupPtr& group( std::dynamic_pointer_cast< Group >( node ) );
	
	// Yes it is.
	if( group )
	{
		// Cull the children nodes recursively.
		foreach( const NodePtr& child, group->getChildren() )
		{
			if( !child->isVisible() ) continue;
			cull( block, child );
		}
	}

	// If it is a renderable object, then we perform frustum culling
	// and if the node is visible, then we push it to a list of things
	// to render that will be later passed to the rendering device.
	const TransformPtr& transform = node->getTransform();
	foreach( const GeometryPtr& geometry, node->getGeometry() ) 
	{
		// No frustum culling is performed yet.
		// TODO: Hack! :D
		geometry->appendRenderables( block.renderables, transform );
	}

	const LightPtr& light = node->getComponent<Light>("Light");
	
	if( light ) 
	{
		LightState ls;
		ls.light = light;
		ls.transform = transform;
	
		block.lights.push_back( ls );
	}

	foreach( const ComponentMapPair& cmp, node->getComponents() )
	{
		if( !cmp.second->isDebugRenderableVisible() )
			continue;

		RenderState renderState;
		
		renderState.renderable = cmp.second->getDebugRenderable();
		renderState.modelMatrix = transform->getAbsoluteTransform();
		renderState.group = RenderGroup::Normal;
		renderState.priority = 0;

		block.renderables.push_back( renderState );
	}
}

//-----------------------------------//

void Camera::serialize( Json::Value& value )
{
	value["projection"] = projection == Projection::Orthographic ?
		"Orthographic" : "Perspective"; /*Projection::toString( projection );*/
	value["fov"] = fov;
	value["near"] = near_;
	value["far"] = far_;
	value["lookAt"] = toJson(lookAtVector);
}

//-----------------------------------//

} } // end namespaces