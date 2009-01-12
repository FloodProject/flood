/************************************************************************
*
* vaporEngine by triton © (2008-2009)
*
*	<http://www.portugal-a-programar.org/
*
************************************************************************/

#pragma once

namespace vapor {
	namespace render {
		// Same goes for IBs
		namespace BufferAccessUsage
		{
			enum BufferAccessUsage
			{
				WRITE_ONLY = 0, // Write content only
				READ_ONLY, // Read content only
				READ_WRITE // Read and write content
			};
		};

		namespace BufferCreationUsage
		{
			enum BufferCreationUsage
			{
				STREAM_DRAW, // Stream draw
				STREAM_READ, // Stream read
				STREAM_COPY, // Stream copy
				STATIC_DRAW, // Static draw
				STATIC_READ, // Static read
				STATIC_COPY, // Static copy
				DYNAMIC_DRAW, // Dynamic draw
				DYNAMIC_READ, // Dynamic read
				DYNAMIC_COPY // Dynamic copy
			};
		};

		namespace VertexUsage
		{
			enum VertexUsage
			{
				NONE = 0, // No usage
				POSITION, // Position
				TEXCOORD, // Texture Coordinate
				NORMAL // Normal
			};
		};

		namespace VertexDataType
		{
			enum VertexDataType
			{
				FLOAT1, // 1 Float
				FLOAT2, // 2 Floats (vec2)
				FLOAT3, // 3 Floats (vec3)
				FLOAT4 // 4 Floats (vec4)
			};
		};

struct VertexElement
{
	unsigned long Offset;
	unsigned long DataType;
	unsigned long Usage;
};

#define VERTEXELEMENT_END {0, 0, VertexUsage::NONE}

class VertexBuffer
{
public:
	virtual ~VertexBuffer(void) {};

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
};

}} // end namespaces
