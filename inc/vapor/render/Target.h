/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

namespace vapor { namespace render {

//-----------------------------------//

class VAPOR_API Settings
{
public:

	Settings( const ushort width = 640, const ushort height = 480 )
		: width( width ), height( height )
	{ }

	// Gets the width of the target.
	const ushort getWidth() const { return width; }
	
	// Gets the height of the target.
	const ushort getHeight() const { return height; }

	// Sets the width of the target.
	void setWidth(ushort w) { width = w; }
	
	// Gets the height of the target.
	void setHeight(ushort h) { height = h; }

public:

	ushort width, height;
};

//-----------------------------------//

/**
 * Render target. Windows, FBOs, etc.
 */

class VAPOR_API RenderTarget
{
public:

	RenderTarget() { }
	virtual ~RenderTarget() { }
	
	// Updates the render target (usually swaps buffers).
	virtual void update() = 0;

	// Sets this rendering target as the current.
	virtual void makeCurrent() = 0;

	// Gets the settings of this render target.
	virtual const Settings& getSettings() = 0;

	// Event fired when the target gets resized.
	fd::delegate< void( const Settings& ) > onTargetResize;
};

//-----------------------------------//

TYPEDEF_SHARED_POINTER_FROM_CLASS( RenderTarget );

//-----------------------------------//

} } // end namespaces
