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
#include "vapor/render/Device.h"
#include "vapor/render/View.h"
#include "vapor/Engine.h"

namespace vapor {

//-----------------------------------//

BEGIN_CLASS_PARENT(Camera, Component)
	FIELD_CLASS(Camera, Frustum, frustum)
END_CLASS()

//-----------------------------------//

Camera::Camera()
{
	init();

	renderDevice = Engine::getInstancePtr()->getRenderDevice();
}

//-----------------------------------//

Camera::Camera( RenderDevice* device )
	: renderDevice(device)
{
	assert( device != nullptr );
	init();
}

//-----------------------------------//

Camera::Camera( const Camera& rhs )
	: renderDevice( rhs.renderDevice )
{ }

//-----------------------------------//

Camera::~Camera()
{
	assert( transform != nullptr );
	transform->onTransform -= fd::bind( &Camera::onTransform, this );
}

//-----------------------------------//

void Camera::init()
{
	activeView = nullptr;
	lookAtVector = Vector3::UnitZ;

	frustum.projectionType = Projection::Perspective;
	frustum.fieldOfView = 45;
	frustum.nearPlane = 1;
	frustum.farPlane = 5000;
}

//-----------------------------------//

void Camera::updateViewTransform()
{
	assert( transform != nullptr );

	const Vector3& position = transform->getPosition();
	const Quaternion& rotation = transform->getRotation();
	
	// Update the look-at vector.
	Vector3 forward = Matrix4x3::createFromQuaternion(rotation) * Vector3::UnitZ;
	lookAtVector = position + forward;
	
	// Update the view matrix.
	viewMatrix = transform->lookAt( lookAtVector, Vector3::UnitY );

	// Update the frustum planes.
	frustum.updatePlanes( viewMatrix );
}

//-----------------------------------//

void Camera::setView( View* view )
{
	if( !view )
		return;

	if( activeView == view )
		return;

	activeView = view;

	// Update frustum matrices.
	frustum.aspectRatio = view->getAspectRatio();
	frustum.updateProjection( view->getSize() );
	frustum.updatePlanes( viewMatrix );	
}

//-----------------------------------//

void Camera::update( double VAPOR_UNUSED(delta) )
{
	if( activeView )
	{
		// Update frustum matrices.
		frustum.aspectRatio = activeView->getAspectRatio();
		frustum.updateProjection( activeView->getSize() );
		frustum.updatePlanes( viewMatrix );	
	}

	// Only run the following code once.
	if( transform )
		return;
		
	transform = getNode()->getTransform();
	transform->onTransform += fd::bind( &Camera::onTransform, this );

	// Update the view transform the first update.
	updateViewTransform();
}

//-----------------------------------//

void Camera::onTransform()
{
	updateViewTransform();
}

//-----------------------------------//

void Camera::render( const NodePtr& node, bool clearView )
{
	if( !activeView )
		return;

	// This will contain all nodes used for rendering.
	RenderBlock renderBlock;

	// Perform frustum culling.
	cull( renderBlock, node );
	
	renderDevice->setView( activeView );

	if( clearView )
		renderDevice->clearView();

	renderDevice->render( renderBlock );
}

//-----------------------------------//

void Camera::render()
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
		#pragma TODO("Fix multiple geometry instancing")
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
		renderState.group = RenderStage::PostTransparency;
		renderState.priority = 0;

		block.renderables.push_back( renderState );
	}
}

//-----------------------------------//

Ray Camera::getRay( float screenX, float screenY, Vector3* outFar ) const
{
	assert( activeView != nullptr );
	Vector2i size = activeView->getSize();

	Vector3 nearPoint(screenX, size.y - screenY, 0);
	Vector3 farPoint (screenX, size.y - screenY, 1);

	const Matrix4x4& matProjection = frustum.projectionMatrix;

	Vector3 rayOrigin =
		activeView->unprojectPoint(nearPoint, matProjection, viewMatrix);
	
	Vector3 rayTarget =
		activeView->unprojectPoint(farPoint, matProjection, viewMatrix);

	Vector3 rayDirection = rayTarget - rayOrigin;
	rayDirection.normalize();

	Ray pickRay(rayOrigin, rayDirection);

	if( outFar )
		*outFar = rayTarget;

	return pickRay;
}

//-----------------------------------//

} // end namespace