/************************************************************************
*
* vaporEngine by triton © (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

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

	BufferManager() {};
	virtual ~BufferManager() {};

	// creates a Vertex Buffer bound to the given data
	virtual shared_ptr<VertexBuffer> createVertexBuffer(uint numElements, 
		VertexDeclaration decl, BufferUsage::Enum bue, BufferType::Enum bte) = 0;

	// creates a Index Buffer bound to the given data
	//virtual shared_ptr<IndexBuffer> createIndexBuffer() = 0;

	//// bind a vertex buffer (0 to unbind, non-0 to bind)
	//virtual void bindVertexBuffer(VertexBuffer *Buffer) = 0;

	//// bind an index buffer (0 to unbind, non-0 to bind)
	//virtual void bindIndexBuffer(IndexBuffer *Buffer) = 0;
};

} } // end namespaces
