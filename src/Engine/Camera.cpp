/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "scene/Camera.h"
#include "scene/Scene.h"
#include "scene/Geometry.h"
#include "render/Device.h"
#include "render/View.h"
#include "render/DebugGeometry.h"
#include "Engine.h"

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

void Camera::update( double )
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
	Vector3 rayOrigin = activeView->unprojectPoint(nearPoint, matProjection, viewMatrix);
	Vector3 rayTarget = activeView->unprojectPoint(farPoint, matProjection, viewMatrix);
	
	Vector3 rayDirection = rayTarget - rayOrigin;
	rayDirection.normalize();

	if( outFar != nullptr )
		*outFar = rayTarget;

	Ray pickRay(rayOrigin, rayDirection);
	return pickRay;
}

//-----------------------------------//

Frustum Camera::getVolume( float screenLeft, float screenRight, float screenTop, float screenBottom )
{
	const TransformPtr& transform = getEntity()->getTransform();
	const Vector3& pos = transform->getPosition();

	Frustum volume;

	if(frustum.projection == Projection::Perspective)
	{
		Ray ul = getRay(screenLeft, screenTop);
		Ray ur = getRay(screenRight, screenTop);
		Ray bl = getRay(screenLeft, screenBottom);
		Ray br = getRay(screenRight, screenBottom);

		Vector3 normal;
		
		// Planes order: Left, Right, Top, Bottom, Near, Far.

		// Left plane
		normal = bl.direction.cross(ul.direction);
		volume.planes[0] = Plane(normal, pos);

		// Right plane
		normal = ur.direction.cross(br.direction);
		volume.planes[1] = Plane(normal, pos);

		// Top plane
		normal = ul.direction.cross(ur.direction);
		volume.planes[2] = Plane(normal, pos);

		// Bottom plane
		normal = br.direction.cross(bl.direction);
		volume.planes[3] = Plane(normal, pos);
	}
	else
	{
		// Ortho planes are parallel to frustum planes
		Ray ul = getRay(screenLeft, screenTop);
		Ray br = getRay(screenRight, screenBottom);

		volume.planes[0] = Plane(frustum.planes[0].normal, ul.origin);	
		volume.planes[1] = Plane(frustum.planes[1].normal, br.origin);
		volume.planes[2] = Plane(frustum.planes[2].normal, ul.origin);
		volume.planes[3] = Plane(frustum.planes[3].normal, br.origin);
	}

	// Near & Far plane applicable to both projection types
	volume.planes[4] = frustum.planes[4];
	volume.planes[5] = frustum.planes[5];

	return volume;
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