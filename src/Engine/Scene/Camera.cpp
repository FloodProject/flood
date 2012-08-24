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
#include "Graphics/RenderDevice.h"
#include "Graphics/RenderView.h"
#include "Geometry/DebugGeometry.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(Camera, Component)
	FIELD_CLASS(4, Frustum, frustum)
	FIELD_PRIMITIVE(5, bool, frustumCulling)
REFLECT_CLASS_END()

//-----------------------------------//

Camera::Camera()
	: activeView(nullptr)
	, frustumCulling(false)
	, transform(nullptr)
	, lookAtVector(Vector3::UnitZ)
{
}

//-----------------------------------//

Camera::~Camera()
{
	if( !transform ) return;
	transform->onTransform.Disconnect( this, &Camera::onTransform );
}

//-----------------------------------//

static Vector3 CalculateLookAtVector(Transform* transform)
{
	const Vector3& position = transform->getPosition();
	const Quaternion& rotation = transform->getRotation();
	
	// Calculate the transform forward vector.
	Vector3 forward = Matrix4x3::createFromQuaternion(rotation) * Vector3::UnitZ;
	
	return position + forward;
}

//-----------------------------------//

void Camera::updateViewTransform()
{
	assert( transform != nullptr );
	lookAtVector = CalculateLookAtVector(transform);
	
	// Update the view matrix.
	viewMatrix = transform->lookAt( lookAtVector, Vector3::UnitY );

	// Update the frustum planes.
	frustum.updatePlanes( viewMatrix );
}

//-----------------------------------//

void Camera::updateFrustum()
{
	if( !activeView ) return;

	// Update frustum matrices.
	frustum.aspectRatio = activeView->getAspectRatio();
	frustum.orthoSize = activeView->getSize();
	frustum.updateProjection();
	frustum.updatePlanes( viewMatrix );
}

//-----------------------------------//

void Camera::setView( RenderView* view )
{
	if( !view ) return;

	activeView = view;
	activeView->viewMatrix = viewMatrix;
	activeView->projectionMatrix = frustum.matProjection;

	updateFrustum();
}

//-----------------------------------//

void Camera::update( float )
{
	if( !activeView ) return;

	drawer.reset();
	bool frustumUpdated = false;

	if( !frustumUpdated)
		updateFrustum();

	// Only run the following code once.
	if( transform ) return;

	transform = getEntity()->getTransform().get();
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

void Camera::render( const Scene* scene )
{
	// This will contain all nodes used for rendering.
	RenderBlock renderBlock;

	// Perform frustum culling.
	cull( renderBlock, &scene->entities );

	// Submits the geometry to the renderer.
	render( renderBlock );
}

//-----------------------------------//

void Camera::render( RenderBlock& block, bool clearView )
{
	if( !activeView ) return;
	
	RenderDevice* renderDevice = GetRenderDevice();
	renderDevice->setActiveView( activeView );

	if( clearView )
		renderDevice->clearView();

	block.renderables.insert(
		block.renderables.begin(),
		drawer.renderables.begin(),
		drawer.renderables.end() );

	renderDevice->render( block );
}

//-----------------------------------//

void Camera::cull( RenderBlock& block, const Entity* entity )
{
	if( !entity ) return;

	// Try to see if this is a Group-derived node.
	Class* klass = entity->getType();
	
	if( ClassInherits(klass, ReflectionGetType(Group)) )
	{
		const Group* group = (Group*) entity;

		const std::vector<EntityPtr>& entities = group->getEntities();

		// Cull the children entities recursively.
		for( size_t i = 0; i < entities.size(); i++ )
		{
			const Entity* child = entities[i].get();
			cull( block, child );
		}

		return;
	}

	// If this is a visible.renderable object, then we perform frustum culling
	// and then we push it to a list of things passed later to the renderer.

	//entity->onPreCull();

	if( !entity->isVisible() )
		return;

	const Transform* transform = entity->getTransform().get();
	const BoundingBox& box = transform->getWorldBoundingVolume();

	bool isCulled = !entity->getTag(Tags::NonCulled);
	
	if( frustumCulling && isCulled && !frustum.intersects(box) )
		return;

	#pragma TODO("Fix multiple geometry instancing")

	const std::vector<GeometryPtr>& geoms = entity->getGeometry();

	for( size_t i = 0; i < geoms.size(); i++ )
	{
		const GeometryPtr& geometry = geoms[i];
		geometry->appendRenderables( block.renderables, transform );
	}

#if 0
	const LightPtr& light = entity->getComponent<Light>();
	
	if( light ) 
	{
		LightState ls;
		ls.light = light.get();
		ls.transform = transform.get();
	
		block.lights.push_back( ls );
	}
#endif

#ifdef BUILD_DEBUG
	const ComponentMap& components = entity->getComponents();
	ComponentMap::const_iterator it;
	
	for( it = components.begin(); it != components.end(); it++ )
	{
		const ComponentPtr& component = it->second;
		component->onPreRender(*this);

		if( !component->isDebugRenderableVisible() )
			continue;

		DebugDrawFlags::Enum flags = (DebugDrawFlags::Enum) 0;
		component->onDebugDraw(drawer, flags);
	}
#endif
}

//-----------------------------------//

static Vector3 UnprojectViewPoint( const Vector3& screen, const RenderView* view, const Camera* camera )
{
	Matrix4x4 matView = camera->getViewMatrix();
	const Matrix4x4& matProjection = camera->getFrustum().matProjection;
	Matrix4x4 matInverseViewProjection = (matView * matProjection).inverse();

	const Vector2i& size = view->getSize();

	// Map x and y from window coordinates, map to range -1 to 1.

	Vector4 pos;
	pos.x = (screen.x /*- offset.x*/) / float(size.x) * 2.0f - 1.0f;
	pos.y = (screen.y /*- offset.y*/) / float(size.y) * 2.0f - 1.0f;
	pos.z = screen.z * 2.0f - 1.0f;
	pos.w = 1.0f;
 
	Vector4 pos2 = matInverseViewProjection * pos;
	Vector3 out( pos2.x, pos2.y, pos2.z );
 
	return out / pos2.w;
}

//-----------------------------------//

Ray Camera::getRay( float screenX, float screenY, Vector3* outFar ) const
{
	//assert( activeView != nullptr );
	if( !activeView ) return Ray();
	
	Vector2i size = activeView->getSize();

	Vector3 nearPoint(screenX, size.y - screenY, 0);
	Vector3 farPoint (screenX, size.y - screenY, 1);

	Vector3 rayOrigin = UnprojectViewPoint(nearPoint, activeView, this);
	Vector3 rayTarget = UnprojectViewPoint(farPoint, activeView, this);
	
	Vector3 rayDirection = rayTarget - rayOrigin;
	rayDirection.normalize();

	if( outFar != nullptr ) *outFar = rayTarget;

	Ray pickRay(rayOrigin, rayDirection);
	return pickRay;
}

//-----------------------------------//

Frustum Camera::getVolume( float screenLeft, float screenRight, float screenTop, float screenBottom )
{
	const Transform* transform = getEntity()->getTransform().get();
	const Vector3& pos = transform->getPosition();

	Frustum volume;

	if(frustum.projection == FrustumProjection::Perspective)
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

static Frustum CalculateWorldFrustum(const Transform* transform, Frustum local)
{
	const Matrix4x3& absolute = transform->getAbsoluteTransform();

	for(size_t i = 0; i < ARRAY_SIZE(local.corners); i++)
		local.corners[i] = absolute * local.corners[i];

	return local;
}

void Camera::onDebugDraw( DebugDrawer& debug, DebugDrawFlags::Enum )
{
	debug.drawFrustum( CalculateWorldFrustum(transform, frustum) );
}

//-----------------------------------//

NAMESPACE_ENGINE_END