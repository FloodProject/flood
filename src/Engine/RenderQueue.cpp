/************************************************************************
*
* vaporEngine by triton (2008)
*
*	<http://www.portugal-a-programar.org/
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
