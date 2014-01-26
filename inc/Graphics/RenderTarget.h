/************************************************************************
*
* Flood Project � (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Graphics/API.h"
#include "Graphics/RenderContext.h"
#include "Core/Event.h"
#include "Core/Math/Vector.h"

NAMESPACE_GRAPHICS_BEGIN

//-----------------------------------//

class RenderView;

class API_GRAPHICS FLD_VALUE_TYPE Settings
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

typedef Vector<RenderView*> RenderViewsVector;

class API_GRAPHICS RenderTarget
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
	GETTER(Context, RenderContext*, context.get())

	// Sets the context associated with the target.
	void setContext(RenderContext* context);

	// Gets the list of views associated with the target.
	GETTER(Views, const RenderViewsVector&, views)

	// Gets/sets custom user data.
	ACCESSOR(UserData, void*, userData)

	// Event fired when the target gets resized.
	Event1<const Settings&> onTargetResize;

protected:

	// Handles resizing the target views.
	void handleResize();

	RenderContextPtr context;
	RenderViewsVector views;

	// User data.
	void* userData;
};

//-----------------------------------//

NAMESPACE_GRAPHICS_END