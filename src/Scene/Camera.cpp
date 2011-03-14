/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"
#include "Scene/Camera.h"
#include "Scene/Scene.h"
#include "Scene/Geometry.h"
#include "Scene/Tags.h"
#include "Render/Device.h"
#include "Render/View.h"
#include "Render/DebugGeometry.h"
#include "Engine.h"

namespace vapor {

//-----------------------------------//

BEGIN_CLASS_PARENT(Camera, Component)
	FIELD_CLASS(Frustum, frustum)
	FIELD_PRIMITIVE(bool, frustumCulling)
END_CLASS()

//-----------------------------------//

Camera::Camera()
	: activeView(nullptr)
	, frustumCulling(false)
	, lookAtVector(Vector3::UnitZ)
{
	renderDevice = GetEngine()->getRenderDevice();
}

//-----------------------------------//

Camera::~Camera()
{
	if( transform )
	{
		transform->onTransform.Disconnect( this, &Camera::onTransform );
		transform.reset();
	}
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

void Camera::updateFrustum()
{
	if( !activeView )
		return;

	// Update frustum matrices.
	frustum.aspectRatio = activeView->getAspectRatio();
	frustum.updateProjection( activeView->getSize() );
	frustum.updatePlanes( viewMatrix );

	updateDebugRenderable();
}

//-----------------------------------//

void Camera::setView( RenderView* view )
{
	if( !view || (activeView == view) )
		return;

	activeView = view;

	updateFrustum();
}

//-----------------------------------//

void Camera::update( float )
{
	if( !activeView )
	{
		RenderView* view = renderDevice->getActiveView();
		setView(view);
	}

	updateFrustum();

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

void Camera::render( const ScenePtr& scene )
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

void Camera::cull( RenderBlock& block, const EntityPtr& entity )
{
	// Try to see if this is a Group-derived node.
	const Type& type = entity->getType();
	
	if( type.inherits<Group>() || type.is<Group>() )
	{
		GroupPtr group = std::static_pointer_cast<Group>(entity);
		const std::vector<EntityPtr>& entities = group->getEntities();

		// Cull the children entities recursively.
		for( size_t i = 0; i < entities.size(); i++ )
		{
			const EntityPtr& child = entities[i];			
			cull( block, child );
		}

		return;
	}

	// If this is a visible renderable object, then we perform frustum culling
	// and then we push it to a list of things passed later to the renderer.

	//entity->onPreCull();

	if( !entity->isVisible() )
		return;

	const TransformPtr& transform = entity->getTransform();
	const BoundingBox& box = transform->getWorldBoundingVolume();

	bool performCull = !entity->getTag(Tags::NonCulled);

	if( frustumCulling && performCull && !frustum.intersects(box) )
		return;

	#pragma TODO("Fix multiple geometry instancing")

	const std::vector<GeometryPtr>& geoms = entity->getGeometry();

	for( size_t i = 0; i < geoms.size(); i++ )
	{
		const GeometryPtr& geometry = geoms[i];
		geometry->appendRenderables( block.renderables, transform );
	}

	const LightPtr& light = entity->getComponent<Light>();
	
	if( light ) 
	{
		LightState ls;
		ls.light = light;
		ls.transform = transform;
	
		block.lights.push_back( ls );
	}

	const ComponentMap& components = entity->getComponents();

	ComponentMap::const_iterator it;
	for( it = components.begin(); it != components.end(); it++ )
	{
		const ComponentPtr& component = it->second;

		component->onPreRender(*this);

		if( !component->isDebugRenderableVisible() )
			continue;

		const RenderablePtr& renderable = component->getDebugRenderable();
		
		if( !renderable )
			continue;

		renderable->setRenderLayer(RenderLayer::PostTransparency);

		RenderState renderState;
		renderState.renderable = renderable;

		if( component->getDebugInheritsTransform() )
			renderState.modelMatrix = transform->getAbsoluteTransform();

		block.renderables.push_back( renderState );
	}
}

//-----------------------------------//

Ray Camera::getRay( float screenX, float screenY, Vector3* outFar ) const
{
	assert( activeView != nullptr );
	Vector2 size = activeView->getSize();

	Vector3 nearPoint(screenX, size.y - screenY, 0);
	Vector3 farPoint (screenX, size.y - screenY, 1);

	Vector3 rayOrigin = activeView->unprojectPoint(nearPoint, this);
	Vector3 rayTarget = activeView->unprojectPoint(farPoint, this);
	
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

	updateDebugFrustum( debugRenderable, frustum );
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