/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/render/Target.h"
#include "vapor/render/Viewport.h"

namespace vapor { namespace render {

//-----------------------------------//

RenderTarget::~RenderTarget()
{
	foreach( ViewportPtr viewport, viewports )
		delete viewport;
}

//-----------------------------------//

Vector2i Settings::getSize() const
{
	return Vector2i(width, height);
}

//-----------------------------------//

ViewportPtr RenderTarget::addViewport( scene::CameraPtr camera )
{
	ViewportPtr viewport = new Viewport(camera, this);
	viewports.push_back( viewport );
	return viewport;
}

//-----------------------------------//

} } // end namespaces
