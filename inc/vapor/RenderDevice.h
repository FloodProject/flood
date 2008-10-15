#pragma once

#include "vapor/Window.h"
#include "vapor/RenderQueue.h"

#include "vapor/Matrix.h"

using namespace vapor::math;

namespace vapor {
	namespace render {

class RenderDevice
{

public:

	RenderDevice();
	virtual ~RenderDevice();

	//-----------------//

	// clear buffers
	virtual void clear() = 0;

	// update window
	virtual void update();

	//-----------------//

	// get window
	virtual Window& getWindow() = 0;

	// get adapter information
	//virtual Adapter getAdapter() = 0;

	//-----------------//

	// get projection matrix
	//virtual const Matrix4x3 & getProjectionMatrix () const = 0;

	// set projection matrix
	//virtual void setProjectionMatrix (const Matrix4x3 &proj) = 0;

	// push projection matrix on stack
	//virtual void pushProjectionMatrix (const Matrix4x3 &proj) = 0;

	// pop projection matrix from stack
	//virtual void popProjectionMatrix (bool set = true) = 0;

protected:

	// manages the objects to render
	RenderQueue* renderQueue;

};

} } // end namespaces