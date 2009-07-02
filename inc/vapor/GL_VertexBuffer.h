/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/CompileOptions.h"

#ifdef VAPOR_RENDERER_OPENGL

#include "vapor/render/VertexBuffer.h"

#include "vapor/render/gl/GL_Device.h"
#include "vapor/render/gl/GL.h"

namespace vapor {
	namespace render {
		namespace opengl {

class GLVertexBuffer : public VertexBuffer
{
public:

	GLVertexBuffer(uint numElems, VertexDeclaration decl, BufferUsage::Enum bu, BufferType::Enum bt);

	virtual ~GLVertexBuffer();

	// Map a buffer
	void* map();

	// Unmap a buffer
	void unmap();

private:

	GLuint id;
	vector<byte> data;
};

} } } // end namespaces

#endif