/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"
#include "Render/Device.h"
#include "Render/Target.h"
#include "Render/View.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

RenderTarget::RenderTarget()
	: context(nullptr)
{
}

//-----------------------------------//

RenderTarget::~RenderTarget()
{
	if( !context ) return;

	RenderDevice* renderDevice = GetRenderDevice();
	
	if( renderDevice->getActiveContext() == context )
	{
		// Remove the context from the render device.
		renderDevice->setActiveContext(nullptr);
	}
}

//-----------------------------------//

RenderView* RenderTarget::createView()
{
	RenderView* view = AllocateThis(RenderView);
	view->setRenderTarget(this);
	
	views.push_back( view );
	return view;
}

//-----------------------------------//

void RenderTarget::removeViews()
{
	RenderDevice* renderDevice = GetRenderDevice();

	for( size_t i = 0; i < views.size(); i++ )
	{
		RenderView* view = views[i];

		if( renderDevice->getActiveView() == view )
		{
			// Remove the active view from the render device.
			renderDevice->setActiveView(nullptr);
		}

		Deallocate(view);
	}

	views.clear();
}

//-----------------------------------//

void RenderTarget::setContext(RenderContext* newContext)
{
	context = newContext;
	context->mainTarget = this;
}

//-----------------------------------//

Vector2i Settings::getSize() const
{
	return Vector2i(width, height);
}

//-----------------------------------//

NAMESPACE_ENGINE_END
