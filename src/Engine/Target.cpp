/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/render/Target.h"
#include "vapor/render/View.h"

namespace vapor {

//-----------------------------------//

Vector2i Settings::getSize() const
{
	return Vector2i(width, height);
}

//-----------------------------------//

RenderTarget::~RenderTarget()
{
	foreach( View* view, viewports )
		delete view;
}

//-----------------------------------//

View* RenderTarget::addViewport( const CameraPtr& camera )
{
	View* view = new View(camera, this);
	viewports.push_back( view );
	return view;
}

//-----------------------------------//

} // end namespace
