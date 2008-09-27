#pragma once

#include <cstdio>
#include <vector>
#include <string>
#include <list>

#include "vapor/Milkshape3D.h"

using namespace std;
using vapor::resources::Milkshape3D;

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