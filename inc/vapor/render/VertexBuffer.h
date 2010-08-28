/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/math/Vector3.h"
#include "vapor/render/Buffer.h"

namespace vapor {

//-----------------------------------//

/**
 * Attribute of a vertex element.
 */

namespace VertexAttribute
{
	enum Enum
	{
		Position = 0,
		Normal = 2,
		Color = 3,
		SecondaryColor = 4,
		FogCoord = 5,
		TexCoord0 = 8,
		TexCoord1,
		TexCoord2,
		TexCoord3,
		TexCoord4,
		TexCoord5,
		TexCoord6,
		TexCoord7,
	};
}

//-----------------------------------//

struct Attribute
{
	Attribute()
	{ }

	Attribute(const Attribute& rhs)
	  : components(rhs.components)
	  , size(rhs.size)
	  , data(rhs.data)
	  , type(rhs.type)
	{ }

	int components;
	int size;
	int type;
	std::vector<byte> data;
};

//-----------------------------------//

/**
 * Represents a vertex buffer. One limitation here is that all data is 
 * tied to the vertex so if you want a normal per primitive and not per 
 * vertex you will have to duplicate that normal for each vertex for now.
 */

class VAPOR_API VertexBuffer : public Buffer
{
public:

	VertexBuffer(BufferUsage::Enum = BufferUsage::Static,
		BufferAccess::Enum = BufferAccess::Write);
	
	virtual ~VertexBuffer();

	// Updates the internal buffer with current attributes.
	bool build();

	// Returns if the this buffer is valid.
	bool isValid() const;

	// Returns true if the vertex buffer is built, false otherwhise.
	bool isBuilt() const;

	// Forces a rebuild of the vertex buffer the next update.
	void forceRebuild();
	
	// Binds the vertex buffer.
	bool bind();
	
	// Unbinds the vertex buffer.
	bool unbind();
	
	// Clears this vertex buffer. All vertex data will be gone.
	void clear();

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
	bool set( VertexAttribute::Enum attr, const std::vector< Vector3 >& data );

	// Sets the attribute data.
	bool set( VertexAttribute::Enum attr, const std::vector<float>& data );

private:

	// Checks that each entry in the map has the same size.
	bool checkSize() const;

	// Binds all the OpenGL pointers when the buffer is built.
	void bindPointers();

	// Tells us if this buffer has already been built.
	bool built;

	// Used to store specific GL types for each attribute.
	typedef std::map< VertexAttribute::Enum, Attribute > AttributeMap;
	typedef std::pair< const VertexAttribute::Enum, Attribute > AttributeMapPair;
	
	// Holds all the vertex attributes.
	mutable AttributeMap attributes;

	// Holds the number of vertices inside.
	mutable uint numVertices;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( VertexBuffer );

//-----------------------------------//

} // end namespace