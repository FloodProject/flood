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

	void lookAt(Vec3f pos);

private:

	// Frustum frustum;

};

} } // end namespaces