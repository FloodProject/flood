/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "render/Target.h"
#include "render/View.h"

namespace vapor {

//-----------------------------------//

RenderTarget::RenderTarget()
{ }

//-----------------------------------//

RenderTarget::~RenderTarget()
{
	for( uint i = 0; i < views.size(); i++ )
	{
		const RenderView* view = views[i];
		delete view;
	}
}

//-----------------------------------//

RenderView* RenderTarget::createView()
{
	RenderView* view = new RenderView();
	view->setRenderTarget(this);

	views.push_back( view );
	return view;
}

//-----------------------------------//

Vector2i Settings::getSize() const
{
	return Vector2i(width, height);
}

//-----------------------------------//

} // end namespace
