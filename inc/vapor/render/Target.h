/************************************************************************
*
* vaporEngine by triton © (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/render/Queue.h"

namespace vapor {
	namespace render {

class RenderTarget
{
public:

	RenderTarget() { }
	virtual ~RenderTarget() { }
	
	virtual void update() = 0;

private:

	// manages the objects to render
	RenderQueue* renderQueue;
};

} } // end namespaces
