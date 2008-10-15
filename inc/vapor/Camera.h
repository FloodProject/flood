#pragma once

#include "vapor/Vector3.h"

using namespace vapor::math;

namespace vapor {
	namespace render {

class Camera
{
public:

	Camera();
	~Camera();

	void lookAt(Vector3f pos);

private:

	// Frustum frustum;
	int zoomX, zoomY;

};

} } // end namespaces