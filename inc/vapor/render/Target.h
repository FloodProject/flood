/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/math/Vector2.h"

namespace vapor { namespace render {

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
	IMPLEMENT_ACESSOR(Width, const ushort, width)

	// Gets/sets the height of the target.
	IMPLEMENT_ACESSOR(Height, const ushort, height)

	// Gets the size of the target.
	math::Vector2i getSize() const;

public:

	ushort width, height;
};

//-----------------------------------//

/**
 * Render target. Windows, FBOs, etc.
 */

class VAPOR_API RenderTarget : private boost::noncopyable
{
public:

	RenderTarget() { }
	virtual ~RenderTarget() { }
	
	// Updates the render target (usually swaps buffers).
	virtual void update() = 0;

	// Sets this rendering target as the current.
	virtual void makeCurrent() = 0;

	// Gets the settings of this render target.
	virtual const Settings& getSettings() const = 0;

	// Event fired when the target gets resized.
	fd::delegate< void( const Settings& ) > onTargetResize;
};

//-----------------------------------//

} } // end namespaces
