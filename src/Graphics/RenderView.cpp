/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Graphics/API.h"
#include "Graphics/RenderView.h"
#include "Graphics/RenderTarget.h"

NAMESPACE_GRAPHICS_BEGIN

//-----------------------------------//

RenderView::RenderView()
	: origin(0, 0)
	, size(0, 0)
	, depthPriority(0)
	, target(nullptr)
{
	setClearColor( Color::White );
}

//-----------------------------------//

RenderView::~RenderView()
{
}

//-----------------------------------//

void RenderView::handleRenderTargetResize()
{
	#pragma TODO("Views need to be updated when render targets change")
	
#if 0
	//size = target->getSettings().getSize();

	const CameraPtr& camera = weakCamera;
	if( !camera ) return;

	camera->updateFrustum();
#endif
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

#if 0
void RenderView::setCamera( const CameraPtr& camera )
{
	weakCamera = camera.get();
	if( !weakCamera ) return;

	onCameraChanged(camera);
}
#endif

//-----------------------------------//

bool RenderView::operator < (RenderView& v)
{
	return getDepthPriority() < v.getDepthPriority();
}

//-----------------------------------//
#if 0
void RenderView::update(const Scene* scene)
{
	if( !scene ) return;

	const CameraPtr& camera = weakCamera;
	if( !camera ) return;

	camera->setView(this);
	camera->render(scene);
}
#endif
//-----------------------------------//

NAMESPACE_GRAPHICS_END