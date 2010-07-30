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

namespace vapor {

//-----------------------------------//

const std::string& Camera::type = "Camera";

//-----------------------------------//

std::string Projection::toString( Projection::Enum proj )
{
	switch(proj)
	{
	case Projection::Orthographic:
		return "Ortographic";
	case Projection::Perspective:
		return "Perspective";
	default:
		assert( false );
	}
}

//-----------------------------------//

Camera::Camera( RenderDevice* device, Projection::Enum proj )
	: lookAtVector(Vector3::UnitZ)
	, projection(proj)
	, fov(45.0f)
	, near_(1.0f)
	, far_(5000.0f)
	, viewport(nullptr)
	, viewSize(Vector2i::Zero)
	, renderDevice(device)
{
	assert( device != nullptr );
}

//-----------------------------------//

Camera::Camera( const Camera& rhs )
	: renderDevice( rhs.renderDevice )
	, projection( rhs.projection )
	, fov( rhs.fov )
	, near_( rhs.near_ )
	, far_( rhs.far_ )
{ }

//-----------------------------------//

Camera::~Camera()
{
	if( transform )
	{
		transform->onTransform -= fd::bind( &Camera::onTransform, this );
	}
}

//-----------------------------------//

Ray Camera::getRay( float screenX, float screenY, Vector3* outFar ) const
{
	assert( viewport != nullptr );

	Vector3 nearPoint(screenX, screenY, 0);
	Vector3 farPoint(screenX, screenY, 1);

	Vector3 rayOrigin = viewport->Unproject(nearPoint, projectionMatrix, viewMatrix);
	Vector3 rayTarget = viewport->Unproject(farPoint, projectionMatrix, viewMatrix);

	Vector3 rayDirection = rayTarget - rayOrigin;
	rayDirection.normalize();

	Ray pickRay(rayOrigin, rayDirection);

	if( outFar )
		*outFar = rayTarget;

	return pickRay;
}

//-----------------------------------//

void Camera::update( double VAPOR_UNUSED(delta) )
{
	// Only run the following code once.
	if( transform )
		return;

	assert( getNode() != nullptr );
	assert( getNode()->getTransform() != nullptr );
		
	transform = getNode()->getTransform();

	transform->onTransform += fd::bind( &Camera::onTransform, this );

	// Update the view matrix the first update.
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
	assert( transform != nullptr );

	const Vector3& position = transform->getPosition();
	const EulerAngles& rotation = transform->getRotation();
	
	// Update the lookAt vector.
	Vector3 forward = Vector3::UnitZ * Matrix4x3::createOrientation(rotation);
	lookAtVector = position + forward;
	
	// Update the view matrix.
	viewMatrix = transform->lookAt( lookAtVector, Vector3::UnitY );
}

//-----------------------------------//

void Camera::setViewport( Viewport* newViewport )
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
	setupView();
}

//-----------------------------------//

void Camera::render( const NodePtr& node, bool clearView ) const
{
	if( !viewport )
		return;

	// This will contain all nodes used for rendering.
	RenderBlock renderBlock;

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

void Camera::cull( RenderBlock& block, const NodePtr& node ) const
{
	// Try to see if this is a Group-derived node.
	const GroupPtr& group( std::dynamic_pointer_cast<Group>(node) );
	
	// Yes it is.
	if( group )
	{
		// Cull the children nodes recursively.
		foreach( const NodePtr& child, group->getChildren() )
		{
			if( !child->isVisible() ) 
				continue;
			
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
		renderState.group = RenderGroup::PostTransparency;
		renderState.priority = 0;

		block.renderables.push_back( renderState );
	}
}

//-----------------------------------//

void Camera::serialize( Json::Value& value )
{
	value["projection"] = Projection::toString( projection );
	value["fov"] = fov;
	value["near"] = near_;
	value["far"] = far_;
	value["lookAt"] = toJson(lookAtVector);
}

//-----------------------------------//

} // end namespace