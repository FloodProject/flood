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

class RenderTarget
{
public:

	RenderTarget() { }
	virtual ~RenderTarget() { }
	
	virtual void update() = 0;
};

} } // end namespaces
