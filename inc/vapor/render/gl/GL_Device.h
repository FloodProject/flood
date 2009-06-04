/************************************************************************
*
* vaporEngine by triton © (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/CompileOptions.h"

#ifdef VAPOR_RENDERER_OPENGL

#include "vapor/render/Device.h"
#include "vapor/render/gl/GL.h"

namespace vapor {
	namespace render {
		namespace opengl {

class GLDevice : public Device
{
public:

	GLDevice(Settings settings);
	virtual ~GLDevice();

	// Create a new render window
	Window& createWindow(Settings& settings);

	// Close the device
	void close();

	// Clear buffers
	void clearTarget();

	//// bind a vertex buffer (0 to unbind, non-0 to bind)
	//void bindVertexBuffer(VertexBuffer *Buffer);

	//// bind an index buffer (0 to unbind, non-0 to bind)
	//void bindIndexBuffer(IndexBuffer *Buffer);

	//// draw a vertex buffer
	//void draw(unsigned long Mode, unsigned long First, unsigned long Count);

private:

	void resetViewport();

	void checkExtensions();

	//// Binded VB and IB
	//GLVertexBuffer *_bindedVB;
	//IndexBuffer *_bindedIB;
};

} } } // end namespaces

#endif