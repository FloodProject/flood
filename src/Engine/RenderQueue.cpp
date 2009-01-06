/************************************************************************
*
* vaporEngine by triton (2008)
*
*	<http://www.portugal-a-programar.org>
*
* DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
* Version 2, December 2004
* 
* DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
* TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
* 
* 0. You just DO WHAT THE FUCK YOU WANT TO.
*
************************************************************************/

#include "vapor/render/RenderQueue.h"

namespace vapor {
	namespace render {

RenderQueue::RenderQueue()
{
}

RenderQueue::~RenderQueue()
{

}

void RenderQueue::addRenderable(Renderable *obj)
{
	listRenderables.push_back(obj);
}

} } // end namespaces
