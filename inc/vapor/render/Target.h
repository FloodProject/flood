/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

namespace vapor {
	namespace render {

/**
 *
 *
 */

class RenderTarget
{
public:

	RenderTarget() { }
	virtual ~RenderTarget() { }
	
	// Updates the render target (usually swaps buffers).
	virtual void update() = 0;

	// Sets this rendering target as the current.
	virtual void makeCurrent() = 0;
};

} } // end namespaces
