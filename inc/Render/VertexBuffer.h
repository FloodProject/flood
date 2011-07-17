/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Math/Vector.h"
#include "Math/Color.h"
#include "Render/Buffer.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

/**
 * Attribute of a vertex element.
 */

struct API_RENDER VertexAttribute
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

//-----------------------------------//

struct API_RENDER Attribute
{
	Attribute() {}

	Attribute(const Attribute& rhs)
	  : components(rhs.components)
	  , size(rhs.size)
	  , data(rhs.data)
	{}

	int32 components;
	int32 size;
	int32 type;
	std::vector<byte> data;
};

//-----------------------------------//

typedef std::map<VertexAttribute::Enum, Attribute> AttributeMap;
typedef std::pair<const VertexAttribute::Enum, Attribute> AttributeMapPair;

/**
 * Represents a vertex buffer. One limitation here is that all data is 
 * tied to the vertex so if you want a normal per primitive and not per 
 * vertex you will have to duplicate that normal for each vertex for now.
 */

class API_RENDER VertexBuffer : public Buffer
{
public:

	VertexBuffer(
		BufferUsage::Enum = BufferUsage::Static,
		BufferAccess::Enum = BufferAccess::Write);
	
	~VertexBuffer();

	// Binds the vertex buffer.
	bool bind();
	
	// Unbinds the vertex buffer.
	bool unbind();

	// Binds the attributes pointers.
	void bindPointers();

	// Unbinds the attribute pointers.
	void unbindPointers();

	// Binds the attribute pointers.
	void bindGenericPointers();

	// Unbinds the attribute pointers.
	void unbindGenericPointers();
	
	// Clears this vertex buffer. All vertex data will be gone.
	void clear();

	// Returns if the this buffer is valid.
	bool isValid() const;

	// Returns true if the vertex buffer is built, false otherwhise.
	bool isBuilt() const;

	// Updates the internal buffer with current attributes.
	bool build();

	// Forces a rebuild of the vertex buffer the next update.
	void forceRebuild();

	// Returns an attribute data in the vertex buffer.
	std::vector<Vector3>& getAttribute( VertexAttribute::Enum ) const;

	// Returns the vertices attribute in the vertex buffer.
	std::vector<Vector3>& getVertices() const;

	// Returns the total size in bytes of the buffer.
	uint getSize() const;

	// Returns the number of vertex attributes.
	uint getNumAttributes() const;

	// Returns the number of vertices in each attribute.
	uint getNumVertices() const;

	// Sets the attribute data.
	bool set( VertexAttribute::Enum attr, const std::vector<Vector3>& data );

	// Sets the attribute data.
	bool set( VertexAttribute::Enum attr, const std::vector<Color>& data );

	// Sets the attribute data.
	bool set( VertexAttribute::Enum attr, const std::vector<float>& data );

	// Sets the attribute data.
	bool set( VertexAttribute::Enum attr, const std::vector<int>& data );

private:

	// Checks that each entry in the map has the same size.
	bool checkSize() const;
	
	// Holds all the vertex attributes.
	mutable AttributeMap attributes;

	// Holds the number of vertices inside.
	mutable uint numVertices;

	// Tracks if the buffer has been built.
	bool built;
};

//API_RENDER VertexBuffer* VertexBufferCreate();

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( VertexBuffer );

//-----------------------------------//

NAMESPACE_ENGINE_END