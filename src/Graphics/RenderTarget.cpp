/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Graphics/API.h"
#include "Graphics/RenderDevice.h"
#include "Graphics/RenderTarget.h"
#include "Graphics/RenderView.h"

NAMESPACE_GRAPHICS_BEGIN

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

	for(size_t i = 0; i < views.size(); ++i)
	{
		RenderView* view = views[i];
		Deallocate(view);
	}

	Deallocate(context);
}

//-----------------------------------//

RenderView* RenderTarget::createView()
{
	RenderView* renderView = AllocateThis(RenderView);
	renderView->setRenderTarget(this);
	renderView->setSize(getSettings().getSize());
	views.push_back(renderView);
	
	return renderView;
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

void RenderTarget::handleResize()
{
	for( size_t i = 0; i < views.size(); i++ )
	{
		RenderView* view = views[i];
		view->handleRenderTargetResize();
	}

	onTargetResize( getSettings() );
}

//-----------------------------------//

Vector2i Settings::getSize() const
{
	return Vector2i(width, height);
}

//-----------------------------------//

NAMESPACE_GRAPHICS_END
