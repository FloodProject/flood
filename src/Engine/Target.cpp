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

namespace vapor {

//-----------------------------------//

Vector2i Settings::getSize() const
{
	return Vector2i(width, height);
}

//-----------------------------------//

RenderTarget::~RenderTarget()
{
	foreach( Viewport* viewport, viewports )
		delete viewport;
}

//-----------------------------------//

Viewport* RenderTarget::addViewport( const CameraPtr& camera )
{
	Viewport* viewport = new Viewport(camera, this);
	viewports.push_back( viewport );
	return viewport;
}

//-----------------------------------//

} // end namespace
