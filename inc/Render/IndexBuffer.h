/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Render/Buffer.h"

namespace vapor {

//-----------------------------------//

class VAPOR_API IndexBuffer : public Buffer
{
public:

	IndexBuffer( int index = 16 );

	// Sets the indices for the IBO (16-bit version).
	void setData( const std::vector<byte>& data );

	// Gets the number of indexes stored in this buffer.
	int getSize() const;

	// Updates the internal IBO with current values for indices.
	bool build();

	// Binds/unbinds the index buffer from the OpenGL state.
	bool bind();
	bool unbind();

public:

	std::vector<byte> data;
	int indexSize;
	bool isBuilt;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( IndexBuffer );

template<typename T>
void SetIndexBufferData(const IndexBufferPtr& ib, const std::vector<T>& data)
{
	if( data.empty() ) return;

	int sizeInBytes = data.size() * sizeof(data[0]);

	ib->data.resize(sizeInBytes); 
	memcpy(&ib->data[0], &data[0], sizeInBytes);
}

//-----------------------------------//

} // end namespace
