/************************************************************************
*
* vaporEngine by triton (2008)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/math/Vector3.h"

using namespace vapor::math;

namespace vapor {
	namespace render {

class Camera
{
public:

	Camera();
	~Camera();

	void lookAt(Vector3 pos);

private:

	// Frustum frustum;
	int zoomX, zoomY;

};

} } // end namespaces
