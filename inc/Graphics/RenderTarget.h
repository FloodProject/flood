/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Core/Event.h"
#include "Math/Vector.h"
#include "Graphics/RenderContext.h"

NAMESPACE_GRAPHICS_BEGIN

//-----------------------------------//

class RenderView;

class API_RENDER Settings
{
public:

	Settings( const uint16 width = 640, const uint16 height = 480 )
		: width(width)
		, height(height)
	{ }

	Settings( const Settings& s )
		: width(s.width)
		, height(s.height)
	{ }

	// Gets the size of the target.
	Vector2i getSize() const;

	uint16 width, height;
};

//-----------------------------------//

class RenderContext;

/**
 * Render targets are surfaces where the rendered images can be stored
 * and/or displayed. The most common use is windows, but there are also
 * FBOs, which are basically offscreen buffers where you can render to.
 * Each render target mantains a list of viewports.
 */

typedef std::vector<RenderView*> RenderViewsVector;

class API_RENDER RenderTarget
{
	DECLARE_UNCOPYABLE(RenderTarget)

public:

	RenderTarget();
	virtual ~RenderTarget();

	// Creates a new view and adds it to this target.
	RenderView* createView();

	// Removes all the views from the target.
	void removeViews();

	// Sets this rendering target as the current.
	virtual void makeCurrent() = 0;

	// Updates the render target (usually swaps buffers).
	virtual void update() = 0;

	// Gets the settings of this render target.
	virtual const Settings& getSettings() const = 0;

	// Gets the context associated with the target.
	GETTER(Context, RenderContext*, context)

	// Sets the context associated with the target.
	void setContext(RenderContext* context);

	// Gets the list of views associated with the target.
	GETTER(Views, const RenderViewsVector&, views)

	// Event fired when the target gets resized.
	Event1<const Settings&> onTargetResize;

protected:

	// Handles resizing the target views.
	void handleResize();

	RenderContext* context;
	RenderViewsVector views;
};

//-----------------------------------//

NAMESPACE_GRAPHICS_END