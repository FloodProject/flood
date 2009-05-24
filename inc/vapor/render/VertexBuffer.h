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

namespace vapor {
	namespace render {

namespace VertexUsage
{
	enum Enum
	{
		NONE = 0, // No usage
		POSITION, // Position
		TEXCOORD, // Texture Coordinate
		NORMAL // Normal
	};
};

namespace VertexDataType
{
	enum Enum
	{
		FLOAT1, // 1 Float
		FLOAT2, // 2 Floats (vec2)
		FLOAT3, // 3 Floats (vec3)
		FLOAT4  // 4 Floats (vec4)
	};
};

struct VertexElement
{
	unsigned long Offset;
	unsigned long DataType;
	unsigned long Usage;
};

#define VERTEXELEMENT_END {0, 0, VertexUsage::NONE}

class VertexBuffer : public Buffer
{
public:

	VertexBuffer();
	virtual ~VertexBuffer();

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

	BufferCreation::Enum bufferCreation;
};

} } // end namespaces
