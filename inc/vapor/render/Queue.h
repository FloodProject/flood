/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
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
