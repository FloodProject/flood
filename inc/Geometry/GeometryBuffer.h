/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Core/References.h"
#include "Math/Vector.h"
#include "Resources/Buffer.h"

NAMESPACE_RESOURCES_BEGIN

//-----------------------------------//

/**
 * Attribute of a vertex element.
 */

struct API_RESOURCE VertexAttribute
{
	enum Enum
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
};

struct API_RESOURCE VertexDataType
{
	enum Enum
	{
		Byte,
		Float,
		Integer
	};
};

//-----------------------------------//

/**
 * Each element inside a vertex declaration.
 */

struct API_RESOURCE VertexElementP
{
	// Semantic attribute of the element.
	VertexAttribute::Enum attribute;

	// Data type of the element.
	VertexDataType::Enum type;

	// Number of components of the element.
	uint8 components;
};

struct API_RESOURCE VertexElement : public VertexElementP
{
	VertexElement(VertexAttribute::Enum, VertexDataType::Enum, uint8 components);

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

struct API_RESOURCE VertexDeclaration
{
	// Adds a new vertex element.
	void add(VertexAttribute::Enum, int numComponents);

	// Adds a new vertex element.
	void add(const VertexElement&);

	// Adds a new vertex element.
	void add(const VertexElementP&);

	// Resets the vertex elements.
	void reset();

	// Finds the element for the given attribute.
	VertexElement* find(VertexAttribute::Enum) const;

	// Calculates the offset to the given element.
	uint8 getOffset(VertexAttribute::Enum) const;

	// Returns the size of a vertex.
	uint8 getVertexSize() const;

	// Calculates the strides of the elements.
	void calculateStrides();

	std::vector<VertexElement> decls;
};

//-----------------------------------//

/**
 * Represents a buffer with geometry data. You have to associate the vertex
 * data layout to the buffer so it can be used by the engine.
 */

class API_RESOURCE GeometryBuffer : public ReferenceCounted
{
public:

	GeometryBuffer();
	GeometryBuffer(BufferUsage::Enum, BufferAccess::Enum);
	
	~GeometryBuffer();

	// Gets/sets the buffer usage type.
	ACESSOR(BufferUsage, BufferUsage::Enum, usage)

	// Gets/sets the buffer access type.
	ACESSOR(BufferAccess, BufferAccess::Enum, access)

	// Forces rebuild of geometry data.
	void forceRebuild();

	// Clears the buffer.
	void clear();

	// Sets the attribute data in the buffer.
	void set(VertexAttribute::Enum, uint8* data, uint32 size);

	// Sets the data in the buffer.
	void set(uint8* data, uint32 size);

	// Adds data to the buffer.
	void add(uint8* data, uint32 size);

	// Sets the index data in the index buffer.
	void setIndex(uint8* data, uint32 size);

	// Adds index data to the index buffer.
	void addIndex(uint8* data, uint32 size);

	// Adds an index to the index buffer.
	void addIndex(uint16 index);

	// Returns if the buffer has indexed data.
	bool isIndexed() const;

	// Gets a pointer to the attribute data.
	float* getAttribute(VertexAttribute::Enum, uint32 i) const;

	// Gets the stride of the vertex attribute.
	int8 getAttributeStride(VertexAttribute::Enum) const;

	// Gets the number of vertices in the buffer.
	uint32 getSizeVertices() const;

	// Gets the number of indices in the buffer.
	uint32 getSizeIndices() const;

	template<typename T>
	void set(VertexAttribute::Enum attr, const std::vector<T>& data)
	{
		if( data.empty() ) return;

		uint32 sizeInBytes = data.size() * sizeof(T);

		if( !declarations.find(attr) )
		{
			VertexElement decl(attr, VertexDataType::Float, sizeof(T) / sizeof(float));
			decl.stride = 0;
			decl.offset = this->data.size();
			decl.size = sizeInBytes;

			declarations.add(decl);

			add((uint8*)&data.front(), sizeInBytes);
		}
		else
		{
			set(attr, (uint8*)&data.front(), sizeInBytes);
		}
	}
	
	// Buffer usage.
	BufferUsage::Enum usage;
	
	// Buffer access.
	BufferAccess::Enum access;

	// Holds the vertex data.
	std::vector<uint8> data;

	// Holds the index data.
	std::vector<uint8> indexData;

	// Holds the index size in bits.
	uint8 indexSize;

	// Holds if the buffer needs to be rebuilt.
	bool needsRebuild;

	// Hash of the contents of the buffer.
	uint32 hash;

	// Holds the vertex declarations.
	VertexDeclaration declarations;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( GeometryBuffer );

//-----------------------------------//

NAMESPACE_RESOURCES_END