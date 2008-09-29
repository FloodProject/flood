#pragma once

#include "vapor/Window.h"
#include "vapor/RenderQueue.h"

namespace vapor {
	namespace render {

class RenderDevice
{

public:

	RenderDevice();
	virtual ~RenderDevice();

	// Open the device
	virtual void open() = 0;

	// Close the device
	virtual void close() = 0;

	// Clear buffers
	virtual void clear() = 0;

	// Get Render Window
	virtual Window& getWindow() = 0;

	//////////////////
	// Matrices stuff
	//////////////////

	// Get projection matrix
	//const Matrix4 & getProjectionMatrix () const = 0;

	// Set projection matrix
	//virtual void setProjectionMatrix (const Matrix4 &proj) = 0;

	// Push projection matrix on stack
	//void pushProjectionMatrix (const Matrix4 &proj) = 0;

	// Pop projection matrix from stack
	//void popProjectionMatrix (bool set = true) = 0;

protected:

	// Manages the objects to render
	RenderQueue* renderQueue;

};

} } // end namespaces