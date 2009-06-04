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

#include "vapor/render/BufferManager.h"
#include "vapor/render/gl/GL_VertexBuffer.h"

namespace vapor {
	namespace render {
		namespace opengl {

class GLBufferManager : public BufferManager
{
public:

	GLBufferManager();
	virtual ~GLBufferManager();

	// creates a vertex buffer bound to the given data
	virtual shared_ptr<VertexBuffer> createVertexBuffer(uint numElements, 
		VertexDeclaration decl, BufferUsage::Enum bue, BufferType::Enum bte);

	// creates a Index Buffer bound to the given data
	//virtual shared_ptr<IndexBuffer> createIndexBuffer();

private:

	vector<GLVertexBuffer*> vertexBuffers;
};

} } } // end namespaces

#endif