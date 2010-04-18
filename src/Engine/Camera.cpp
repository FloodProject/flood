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
	: renderDevice( device ), projection( proj ), fov(45.0f),
	near_(1.0f), far_(5000.0f), lookAtVector(Vector3::UnitZ),
	viewport(nullptr), viewSize(Vector2i::Zero)
{
	assert( device != nullptr );
}

//-----------------------------------//

math::Ray Camera::getRay( float scrx, float scry, math::Vector3* outFar ) const
{
	// Let's do ray picking...
	// Method based on: http://www.mvps.org/directx/articles/rayproj.htm

	assert( viewport != nullptr );

	// TODO: This doesn't seem right.
	Vector2i targetSize = viewport->getSize();
	
	int width = targetSize.x;
	int height = targetSize.y;

	// Normalizing Screen Coordinates
	float x = width - scrx;
	float y = height - scry;

	float dx = (x / (width * 0.5f) - 1.0f) / viewport->getAspectRatio();
	float dy = 1.0f - y / (height * 0.5f);

	// Scaling Coordinates to the Frustum
	float fov_s = math::degreeToRadian(getFOV()) * 0.5f;
	float s = math::tanf( fov_s );
	dx *= s; 
	dy *= s;

	// Calculating the End Points of the Ray
	Vector3 pNear( dx * -near_, dy * -near_, -near_ );
	Vector3 pFar( dx * -far_, dy * -far_, -far_ );

	// Generating an Inverse of the View Matrix
	const Matrix4x3& invView = inverse( getViewMatrix() );

	// Converting the Ray to World Coordinates
	pNear *= invView;
	pFar *= invView;

	// Construct the picking Ray.
	Ray pickRay( pNear, (pFar - pNear).normalize() );

	if( outFar )
		*outFar = pFar;

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

void Camera::render( ) const
{
	NodePtr parent = getNode()->getParent();

	if( !parent ) return;

	// Search for the root node.
	while ( parent->getParent() )
	{
		parent = parent->getParent();
	}
	  
	render( parent );
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