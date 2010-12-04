/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/scene/Camera.h"
#include "vapor/scene/Scene.h"
#include "vapor/scene/Geometry.h"
#include "vapor/render/Device.h"
#include "vapor/render/View.h"
#include "vapor/render/DebugGeometry.h"
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

	Engine* engine = Engine::getInstancePtr();
	renderDevice = engine->getRenderDevice();
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
	if( transform )
		transform->onTransform.Disconnect( this, &Camera::onTransform );
}

//-----------------------------------//

void Camera::init()
{
	activeView = nullptr;
	lookAtVector = Vector3::UnitZ;
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

void Camera::setView( RenderView* view )
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

	updateDebugRenderable();

	// Only run the following code once.
	if( transform )
		return;
		
	transform = getEntity()->getTransform();
	transform->onTransform.Connect( this, &Camera::onTransform );

	// Update the view transform the first update.
	updateViewTransform();
}

//-----------------------------------//

void Camera::onTransform()
{
	updateViewTransform();
}

//-----------------------------------//

void Camera::render()
{
	EntityPtr node = getEntity();
	assert( node != nullptr );

	// Search for the root node.
	while ( node->getParent() )
		node = node->getParent();
	  
	render( node );
}

//-----------------------------------//

void Camera::render( const EntityPtr& scene )
{
	// This will contain all nodes used for rendering.
	RenderBlock renderBlock;

	// Perform frustum culling.
	cull( renderBlock, scene );

	// Submits the geometry to the renderer.
	render( renderBlock );
}

//-----------------------------------//

void Camera::render( RenderBlock& block, bool clearView )
{
	if( !activeView )
		return;
	
	renderDevice->setView( activeView );

	if( clearView )
		renderDevice->clearView();

	renderDevice->render( block );
}

//-----------------------------------//

void Camera::cull( RenderBlock& block, const EntityPtr& node )
{
	// Try to see if this is a Group-derived node.
	const Type& type = node->getInstanceType();
	
	if( type.inherits<Group>() || type.is<Group>() )
	{
		GroupPtr group = std::static_pointer_cast<Group>(node);

		// Cull the children nodes recursively.
		for( uint i = 0; i < group->getEntities().size(); i++ )
		{
			const EntityPtr& child = group->getEntities()[i];

			if( !child->isVisible() ) 
				continue;
			
			cull( block, child );
		}
	}

	// If it is a renderable object, then we perform frustum culling
	// and if the node is visible, then we push it to a list of things
	// to render that will be later passed to the rendering device.
	const TransformPtr& transform = node->getTransform();
	
	for( uint i = 0; i < node->getGeometry().size(); i++ )
	{
		const GeometryPtr& geometry = node->getGeometry()[i];

		// No frustum culling is performed yet.
		#pragma TODO("Fix multiple geometry instancing")
		geometry->appendRenderables( block.renderables, transform );
	}

	const LightPtr& light = node->getComponent<Light>();
	
	if( light ) 
	{
		LightState ls;
		ls.light = light;
		ls.transform = transform;
	
		block.lights.push_back( ls );
	}

	ComponentMap::const_iterator it;
	for( it = node->getComponents().cbegin(); it != node->getComponents().cend(); it++ )
	{
		const ComponentPtr& component = it->second;

		if( !component->isDebugRenderableVisible() )
			continue;

		const RenderablePtr& renderable = component->getDebugRenderable();
	
		RenderState renderState;
		renderState.renderable = renderable;

		if( component->getDebugInheritsTransform() )
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

	const Matrix4x4& matProjection = frustum.matProjection;

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

void Camera::updateDebugRenderable() const
{
	if( !debugRenderable )
		return;

	updateFrustum( debugRenderable, frustum );
}

//-----------------------------------//

RenderablePtr Camera::createDebugRenderable() const
{
	assert( !debugRenderable );

	debugInheritsTransform = false;
	return buildFrustum( frustum );
}

//-----------------------------------//

} // end namespace