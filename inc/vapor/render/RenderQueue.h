/************************************************************************
*
* vaporEngine by triton (2008)
*
*	<http://www.portugal-a-programar.org/
*
************************************************************************/

#pragma once

#include "vapor/render/Renderable.h"

namespace vapor {
	namespace render {

class RenderQueue
{

public:

	RenderQueue();
	~RenderQueue();

	void addRenderable(Renderable *obj);

private:

	list<Renderable*> listRenderables;
};

} } // end namespaces
