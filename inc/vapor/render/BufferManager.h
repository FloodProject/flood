/************************************************************************
*
* vaporEngine by triton © (2008-2009)
*
*	<http://www.portugal-a-programar.org/
*
************************************************************************/

#pragma once

#include "vapor/Engine.h"

#include "vapor/render/VertexBuffer.h"
#include "vapor/render/IndexBuffer.h"

namespace vapor {
	namespace render {

/**
 * This class manages all the buffers used in the rendering system, 
 * like vertex buffers, index buffers, and other types of buffers.
 */

class BufferManager
{

public:

	BufferManager(void);
	virtual ~BufferManager(void);

	//-----------------------------------//

	// creates a Vertex Buffer bound to the given data
	virtual VertexBuffer* createVertexBuffer() = 0;

	// creates a Index Buffer bound to the given data
	virtual IndexBuffer* createIndexBuffer() = 0;

};

} } // end namespaces
