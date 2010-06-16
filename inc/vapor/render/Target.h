/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/math/Vector2.h"

FWD_DECL_TYPEDEF_SHARED(Camera)
FWD_DECL_TYPEDEF_PTR(Viewport)

namespace vapor {

//-----------------------------------//

class VAPOR_API Settings
{
public:

	Settings( const ushort width = 640, const ushort height = 480 )
		: width( width ), height( height )
	{ }

	Settings( const Settings& s )
		: width( s.width ), height( s.height )
	{ }

	// Gets/sets the width of the target.
	ACESSOR(Width, const ushort, width)

	// Gets/sets the height of the target.
	ACESSOR(Height, const ushort, height)

	// Gets the size of the target.
	Vector2i getSize() const;

public:

	ushort width, height;
};

//-----------------------------------//

typedef std::vector<ViewportPtr> ViewportList;

/**
 * Render targets are surfaces where the rendered images can be stored
 * and/or displayed. The most common use is windows, but there are also
 * FBOs, which are basically offscreen buffers where you can render to.
 * Each render target mantains a list of viewports 
 */

class VAPOR_API RenderTarget : private boost::noncopyable
{
public:

	RenderTarget() { }
	virtual ~RenderTarget();
	
	// Updates the render target (usually swaps buffers).
	virtual void update() = 0;

	// Sets this rendering target as the current.
	virtual void makeCurrent() = 0;

	// Gets the list of viewports associated with the render target.
	GETTER(Viewports, const ViewportList&, viewports)

	// Adds a new viewport to this target.
	ViewportPtr addViewport( CameraPtr camera );

	// Gets the settings of this render target.
	virtual const Settings& getSettings() const = 0;

	// Event fired when the target gets resized.
	fd::delegate< void( const Settings& ) > onTargetResize;

private:

	ViewportList viewports;
};

TYPEDEF_PTR(RenderTarget)

//-----------------------------------//

} // end namespace