/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/render/Queue.h"

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
