/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Math/Vector.h"
#include "Event.h"

FWD_DECL_SHARED(Camera)

namespace vapor {

//-----------------------------------//

class RenderView;

class VAPOR_API Settings
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

/**
 * Render targets are surfaces where the rendered images can be stored
 * and/or displayed. The most common use is windows, but there are also
 * FBOs, which are basically offscreen buffers where you can render to.
 * Each render target mantains a list of viewports 
 */

class VAPOR_API RenderTarget
{
	DECLARE_UNCOPYABLE(RenderTarget)

public:

	RenderTarget();
	virtual ~RenderTarget();

	// Creates a new view and adds it to this target.
	RenderView* createView();

	// Sets this rendering target as the current.
	virtual void makeCurrent() = 0;

	// Updates the render target (usually swaps buffers).
	virtual void update() = 0;

	// Gets the settings of this render target.
	virtual const Settings& getSettings() const = 0;

	// Gets the list of viewports associated with the render target.
	GETTER(Views, const std::vector<RenderView*>&, views)

	// Event fired when the target gets resized.
	Event1<const Settings&> onTargetResize;

protected:

	std::vector<RenderView*> views;
};

//-----------------------------------//

} // end namespace