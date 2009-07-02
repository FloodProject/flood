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

#include "vapor/Platform.h"

#include "vapor/render/Buffer.h"
#include "vapor/render/VertexBuffer.h"
#include "vapor/render/IndexBuffer.h"

namespace vapor {
	namespace render {

/**
 * This class manages all the buffers used in the rendering system, 
 * like vertex buffers, index buffers, and other types of buffers.
 * You must use this class to create the buffers used for rendering!
 */

class BufferManager
{
public:

	BufferManager();
	~BufferManager();

	// creates a vertex buffer bound to the given data
	std::tr1::shared_ptr<VertexBuffer> createVertexBuffer(uint numElements, 
		VertexDeclaration decl, BufferUsage::Enum bue, BufferType::Enum bte);

	// creates a Index Buffer bound to the given data
	//virtual shared_ptr<IndexBuffer> createIndexBuffer();

private:

	std::vector<VertexBuffer*> vertexBuffers;
};

} } // end namespaces

#endif