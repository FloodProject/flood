#include <SDL_opengl.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include "RenderQueue.h"

namespace vapor {
	namespace renderer {

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

void RenderQueue::drawQueue()
{
	glClearColor(1.0f, 0.5f, 0.7f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

} } // end namespaces