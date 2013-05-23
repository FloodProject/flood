/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Graphics/API.h"
#include "Graphics/RenderTarget.h"
#include "Graphics/RenderDevice.h"
#include "Graphics/RenderView.h"

#include "Core/Containers/Array.h"

NAMESPACE_GRAPHICS_BEGIN

//-----------------------------------//

RenderTarget::RenderTarget()
	: context(nullptr)
	, views(*AllocatorGetHeap())
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

	for(size_t i = 0; i < array::size(views); ++i)
	{
		RenderView* view = views[i];
		Deallocate(view);
	}
}

//-----------------------------------//

RenderView* RenderTarget::createView()
{
	RenderView* renderView = AllocateThis(RenderView);
	renderView->setRenderTarget(this);
	renderView->setSize(getSettings().getSize());
	array::push_back(views, renderView);
	
	return renderView;
}

//-----------------------------------//

void RenderTarget::removeViews()
{
	RenderDevice* renderDevice = GetRenderDevice();

	for( size_t i = 0; i < array::size(views); ++i )
	{
		RenderView* view = views[i];

		if( renderDevice->getActiveView() == view )
		{
			// Remove the active view from the render device.
			renderDevice->setActiveView(nullptr);
		}

		Deallocate(view);
	}

	array::clear(views);
}

//-----------------------------------//

void RenderTarget::setContext(RenderContext* newContext)
{
	context = newContext;
}

//-----------------------------------//

void RenderTarget::handleResize()
{
	for( size_t i = 0; i < array::size(views); ++i )
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
