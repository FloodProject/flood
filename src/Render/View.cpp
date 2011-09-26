/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"
#include "Render/View.h"
#include "Render/Target.h"
#include "Scene/Scene.h"
#include "Scene/Camera.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

RenderView::RenderView()
	: depthPriority(0)
	, target(nullptr)
{
	setClearColor( Color::White );
}

//-----------------------------------//

RenderView::RenderView( const CameraPtr& camera )
	: depthPriority(0)
	, target(nullptr)
{
	setClearColor( Color::White );
	setCamera(camera);
}

//-----------------------------------//

void RenderView::handleRenderTargetResize()
{
	//size = target->getSettings().getSize();
	#pragma TODO("Views need to be updated when render targets change")
	
	const CameraPtr& camera = weakCamera;

	if( !camera )
		return;

	camera->updateFrustum();
}

//-----------------------------------//

Vector3 RenderView::unprojectPoint( const Vector3& screen, const Camera* camera ) const
{
	Matrix4x4 view4( camera->getViewMatrix() );
	const Matrix4x4& proj = camera->getFrustum().matProjection;
	Matrix4x4 inverseVP = (view4 * proj).inverse();

	Vector2 size( getSize().x, getSize().y );

    // Map x and y from window coordinates, map to range -1 to 1.

    Vector4 pos;
    pos.x = (screen.x /*- offset.x*/) / float(size.x) * 2.0f - 1.0f;
    pos.y = (screen.y /*- offset.y*/) / float(size.y) * 2.0f - 1.0f;
    pos.z = screen.z * 2.0f - 1.0f;
    pos.w = 1.0f;
 
	Vector4 pos2 = inverseVP * pos;
	Vector3 pos_out( pos2.x, pos2.y, pos2.z );
 
    return pos_out / pos2.w;
}

//-----------------------------------//

float RenderView::getAspectRatio() const
{
	const Vector2i size = getSize();

	if( size.y == 0 )
		return 0;
	
	return float(size.x) / size.y;
}

//-----------------------------------//

void RenderView::setCamera( const CameraPtr& camera )
{
	weakCamera = camera;
	onCameraChanged(camera);
}

//-----------------------------------//

bool RenderView::operator < (RenderView& v)
{
	return getDepthPriority() < v.getDepthPriority();
}

//-----------------------------------//

void RenderView::update(const ScenePtr& scene)
{
	const CameraPtr& camera = weakCamera;
	if( !camera ) return;

	camera->setView(this);
	camera->render(scene);
}

//-----------------------------------//

NAMESPACE_ENGINE_END