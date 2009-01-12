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

#ifdef VAPOR_WINDOWING_SDL
	#define GLEW_STATIC 
	#include <GL/glew.h>

	// needed because SDL_OpenGL conflicts with GLEW
	#define NO_SDL_GLEXT
	#include <SDL_opengl.h>
#endif

#include "vapor/render/Device.h"
#include "vapor/render/SDL_Window.h"

namespace vapor {
	namespace render {
		namespace opengl {

class GLVertexBuffer : public VertexBuffer
{
private:
	unsigned long ID;
	std::vector<unsigned char> Data;
	std::vector<VertexElement> Elements;
public:
	GLVertexBuffer(void);

	virtual ~GLVertexBuffer(void);

	// Map a buffer
	void* map(unsigned long Usage);

	// Unmap a buffer
	void unmap();

	// Create a buffer
	void create(const void* Content, unsigned long ContentLength, 
		unsigned long CreationUsage);

	// Destroy a buffer
	void destroy();

	// Set the vertex elements of a buffer
	void setVertexElements(VertexElement Elements[]);
};

class GLDevice : public Device
{
public:

	GLDevice(WindowSettings& wS);

	virtual ~GLDevice();

	// Open the device
	void open(WindowSettings& wS);

	// Close the device
	void close();

	// Clear buffers
	void clearTarget();

	// bind a vertex buffer (0 to unbind, non-0 to bind)
	void bindVertexBuffer(VertexBuffer *Buffer);

	// bind an index buffer (0 to unbind, non-0 to bind)
	void bindIndexBuffer(IndexBuffer *Buffer);

	// draw a vertex buffer
	void draw(unsigned long Mode, unsigned long First, unsigned long Count);

	// create a Vertex Buffer
	VertexBuffer *createVertexBuffer();

	// create an Index Buffer
	IndexBuffer *createIndexBuffer();

private:

	void resetViewport();

	void checkExtensions();

	std::list<GLVertexBuffer> VertexBuffers;
};

} } } // end namespaces

#endif