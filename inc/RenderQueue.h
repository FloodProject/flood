#pragma once

#include <cstdio>
#include <vector>
#include <string>
#include <list>

#include "Milkshape3D.h"

using namespace std;
using namespace vapor::resources;

namespace vapor {
	namespace renderer {

class RenderQueue
{
public:

	RenderQueue();
	~RenderQueue();

	void addRenderable(Milkshape3D &mesh);
	void drawQueue();

private:

	list<Milkshape3D> listRenderables;
};

} } // end namespaces