/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Core/Math/Vector.h"
#include "Core/Math/Color.h"
#include "Graphics/Buffer.h"

NAMESPACE_GRAPHICS_BEGIN

//-----------------------------------//

/**
 * Attribute of a vertex element.
 */

enum class VertexAttribute : uint8
{
	Position = 0,
	Normal = 2,
	Color = 3,
	BoneIndex = 4,
	FogCoord = 5,
	TexCoord0 = 6,
	TexCoord1,
	TexCoord2,
	TexCoord3,
	TexCoord4,
	TexCoord5,
	TexCoord6,
	TexCoord7,
};

enum class VertexDataType : uint8
{
	Byte,
	Float,
	Integer
};

//-----------------------------------//

/**
 * Each element inside a vertex declaration.
 */

struct API_GRAPHICS VertexElementP
{
	// Semantic attribute of the element.
	VertexAttribute attribute;

	// Data type of the element.
	VertexDataType type;

	// Number of components of the element.
	uint8 components;
};

struct API_GRAPHICS VertexElement : public VertexElementP
{
	VertexElement();
	VertexElement(VertexAttribute, VertexDataType, uint8 components);

	// Returns the size of this element.
	uint8 getSize() const;

	// Stride between elements.
	int8 stride;

	// Offset to the element.
	uint32 offset;

	// Total size of the element.
	uint32 size;
};

 //-----------------------------------//

/**
 * This describes structure of a geometry buffer.
 */

struct API_GRAPHICS VertexDeclaration
{
	VertexDeclaration();

	// Adds a new vertex element.
	void add(VertexAttribute, int numComponents);

	// Adds a new vertex element.
	void add(const VertexElement&);

	// Adds a new vertex element.
	void add(const VertexElementP&);

	// Resets the vertex elements.
	void reset();

	// Finds the element for the given attribute.
	VertexElement* find(VertexAttribute) const;

	// Calculates the offset to the given element.
	uint8 getOffset(VertexAttribute) const;

	// Returns the size of a vertex.
	uint8 getVertexSize() const;

	// Calculates the strides of the elements.
	void calculateStrides();

	Array<VertexElement> decls;
};

//-----------------------------------//

/**
 * Represents a vertex buffer. One limitation here is that all data is 
 * tied to the vertex so if you want a normal per primitive and not per 
 * vertex you will have to duplicate that normal for each vertex for now.
 */

typedef BufferId VertexBufferId;

class API_GRAPHICS VertexBuffer : public Buffer
{
public:

	VertexBuffer();
	~VertexBuffer();
	
	// Returns true if the vertex buffer is built, false otherwhise.
	bool isBuilt() const;

	// Forces a rebuild of the vertex buffer the next update.
	void forceRebuild();

	// Keeps if the buffer has been built.
	bool built;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( VertexBuffer );

//-----------------------------------//

NAMESPACE_GRAPHICS_END