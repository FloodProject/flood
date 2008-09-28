#include <SDL_opengl.h>

#include "vapor/RenderQueue.h"

namespace vapor {
	namespace render {

RenderQueue::RenderQueue()
{
}

RenderQueue::~RenderQueue()
{

}

void RenderQueue::addRenderable(Milkshape3D &mesh)
{
	listRenderables.push_back(mesh);
}

} } // end namespaces