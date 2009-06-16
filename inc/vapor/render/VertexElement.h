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
 * Attribute of a vertex element.
 */

namespace VertexAttribute
{
	enum Enum
	{
		None,
		Position,
		TexCoord,
		Normal,
		Color
	};
}

/**
 * Data type of a vertex element.
 */

namespace VertexDataType
{
	enum Enum
	{
		float1, // Single float value
		float2,	// Two-dimension float vector
		float3, // Three-dimension float vector
		float4  // Four-dimension float vector
	};
}

/**
 * Represents a single vertex element. Vertex elements are used to form
 * a vertex format declaration, and each specify the data type and the
 * layout of the data that makes up a vertice.
 */

struct VertexElement
{
	ulong offset;
	VertexAttribute::Enum vertexAttribute;
	VertexDataType::Enum vertexDataType;
};

typedef vector<VertexElement> VertexDeclaration;

} } // end namespaces
