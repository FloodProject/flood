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

//-----------------------------------//

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

//-----------------------------------//

/**
 * Represents an index buffer.
 */

class VAPOR_API IndexBuffer : public Buffer
{
public:

	IndexBuffer( IndexBufferType::Enum indexType = IndexBufferType::I16bit );
	virtual ~IndexBuffer();

	// Gets the type of indexes of this buffer.
	IndexBufferType::Enum getIndexType() const { return indexType; }

	// Gets the number of indexes stored in this buffer.
	uint getNumIndices() const;

	// Sets the indices for the IBO (16-bit version).
	void set( const std::vector< ushort >& data );
	
	// Sets the indices for the IBO (32-bit version).	
	void set( const std::vector< ulong >& data );

    // This method will make the internal IBO id bound.
    bool bind();

	// Unbinds the IBO from the OpenGL state.
    bool unbind();

    // Updates the internal IBO with current values for indices.
	bool build( BufferUsage::Enum bufferUsage = BufferUsage::Static, 
		BufferAccess::Enum bufferAccess = BufferAccess::Write );

	// Returns true if the vertex buffer is built, false otherwhise.
	bool isBuilt() const;

	// Returns true if this index buffer is 16-bit, false otherwhise.
	bool is16bit() const;

private:

	bool built;
	IndexBufferType::Enum indexType;

	std::vector< ushort > data16;
	std::vector< ulong > data32;
};

//-----------------------------------//

TYPEDEF_SHARED_POINTER_FROM_CLASS( IndexBuffer );

//-----------------------------------//

} } // end namespaces
