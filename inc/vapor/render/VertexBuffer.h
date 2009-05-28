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
#include "vapor/render/VertexElement.h"

namespace vapor {
	namespace render {

/**
 * Represents a vertex buffer.
 */

class VertexBuffer : public Buffer
{
public:

	VertexBuffer() { }
	virtual ~VertexBuffer() { }

	// Map a buffer
	virtual void* map(unsigned long AccessUsage) = 0;
	
	// Unmap a buffer
	virtual void unmap() = 0;
	
	// Create a buffer
	virtual void create(const void* Content, unsigned long ContentLength, 
		unsigned long CreationUsage) = 0;

	// Destroy a buffer
	virtual void destroy() = 0;

	// Set the vertex elements of a buffer
	virtual void setVertexElements(VertexElement Elements[]) = 0;

protected:

	BufferUsage::Enum bufferCreation;
};

} } // end namespaces
