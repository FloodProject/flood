/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"
#include "vapor/render/Buffer.h"

namespace vapor {
	namespace render {

/**
 * Type of the indexes stored in this buffer.
 */

namespace IndexBufferType
{
	enum Enum
	{
		I16bit,
		I32bit
	};
}

/**
 * Represents an index buffer.
 */

class IndexBuffer : public Buffer
{
public:

	IndexBuffer() { }
	virtual ~IndexBuffer() { }

	// Gets the type of indexes of this buffer.
	IndexBufferType::Enum getIndexType() const { return indexType; }

	// Gets the number of indexes stored in this buffer.
	uint getNumIndexes() const { return numIndexes; }

private:

	IndexBufferType::Enum indexType;
	uint numIndexes;
};

} } // end namespaces
