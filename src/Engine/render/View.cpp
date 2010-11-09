/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/render/View.h"
#include "vapor/render/Target.h"
#include "vapor/scene/Camera.h"
#include "vapor/Engine.h"

namespace vapor {

//-----------------------------------//

RenderView::RenderView()
{
	setClearColor( Color::White );
}

//-----------------------------------//

RenderView::RenderView( const CameraPtr& camera )
	: weakCamera( camera )
{
	setClearColor( Color::White );
}

//-----------------------------------//

void RenderView::handleRenderTargetResize()
{
	//size = target->getSettings().getSize();
	#pragma TODO("Views need to be updated when render targets change")
}

//-----------------------------------//

Vector3 RenderView::unprojectPoint( const Vector3& screen, const Matrix4x4& projection,
							  const Matrix4x3& view ) const
{
	Matrix4x4 view4( view );
	Matrix4x4 inverseVP = (view4*projection).inverse();

	Vector2i size = getSize();

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

bool RenderView::operator < (RenderView& v)
{
	return getDepthPriority() < v.getDepthPriority();
}

//-----------------------------------//

void RenderView::update()
{
	CameraPtr camera = weakCamera.lock();

	if( !camera )
		return;

	camera->setView( this );
	camera->render();
}

//-----------------------------------//

} // end namespace