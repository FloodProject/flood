/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/render/Viewport.h"
#include "vapor/scene/Camera.h"
#include "vapor/render/Target.h"

namespace vapor { namespace render {

using namespace vapor::math;
using namespace vapor::scene;

//-----------------------------------//

Viewport::Viewport( CameraPtr camera, RenderTargetPtr rt )
	: weakCamera( camera ), target( rt ), size( Vector2i(-1,-1) )
{
	setClearColor( Color::White );
	setRenderTarget( target );
}

//-----------------------------------//

void Viewport::setRenderTarget( RenderTargetPtr newTarget )
{
	//if( target ) // Remove the old target resize notification.
	//	target->onTargetResize -= fd::bind( &Viewport::handleTargetResize, this );

	//target = newTarget;
	//target->onTargetResize += fd::bind( &Viewport::handleTargetResize, this );
	//handleTargetResize( target->getSettings() );
}

//-----------------------------------//

float Viewport::getAspectRatio() const
{
	const Vector2i size = getSize();

	if( size.y == 0 ) return 0.0f;
	return float(size.x) / size.y;
}

//-----------------------------------//

math::Vector2i Viewport::getSize() const
{
	if( size == Vector2i(-1, -1) )
		return target->getSettings().getSize();
	else
		return size;
}

//-----------------------------------//

bool Viewport::operator < (Viewport& v)
{
	return getPriority() < v.getPriority();
}

//-----------------------------------//

void Viewport::update()
{
	CameraPtr camera( weakCamera );
	camera->setViewport( this );
	camera->render();
}

//-----------------------------------//

} } // end namespaces