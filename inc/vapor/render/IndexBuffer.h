/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/render/Buffer.h"

namespace vapor {

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

	// Gets the type of indexes of this buffer.
	IndexBufferType::Enum getIndexType() const { return indexType; }

	// Gets the number of indexes stored in this buffer.
	uint getNumIndices() const;

	// Gets a reference to the vector with the indices.
	GETTER(Indices16, const std::vector<ushort>&, data16)
	GETTER(Indices32, const std::vector<ulong>&, data32)

	// Sets the indices for the IBO (16-bit version).
	void set( const std::vector<ushort>& data );
	
	// Sets the indices for the IBO (32-bit version).	
	void set( const std::vector<ulong>& data );

	// Binds/unbinds the index buffer from the OpenGL state.
	bool bind();
	bool unbind();

	// Updates the internal IBO with current values for indices.
	bool build();

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

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( IndexBuffer );

//-----------------------------------//

} // end namespace
