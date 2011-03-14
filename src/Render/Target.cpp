/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"
#include "Render/Target.h"
#include "Render/View.h"

namespace vapor {

//-----------------------------------//

RenderTarget::RenderTarget()
{ }

//-----------------------------------//

RenderTarget::~RenderTarget()
{
	for( size_t i = 0; i < views.size(); i++ )
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

Vector2 Settings::getSize() const
{
	return Vector2(width, height);
}

//-----------------------------------//

} // end namespace
