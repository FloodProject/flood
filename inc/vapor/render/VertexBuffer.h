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
#include "vapor/render/GL.h"

namespace vapor { namespace render {

//-----------------------------------//

namespace GLPrimitive
{
	enum Enum
	{
		BYTE	= GL_BYTE,
		UBYTE	= GL_UNSIGNED_BYTE,
		SHORT	= GL_SHORT,
		USHORT	= GL_UNSIGNED_SHORT,
		INT		= GL_INT,
		UINT	= GL_UNSIGNED_INT,
		FLOAT	= GL_FLOAT,
		DOUBLE	= GL_DOUBLE
	};
}

//-----------------------------------//

/**
 * Attribute of a vertex element.
 * Matches the (NVIDIA-only?) specification.
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
        MultiTexCoord0 = 8,
        MultiTexcoord1,
        MultiTexCoord2,
        MultiTexCoord3,
        MultiTexCoord4,
        MultiTexCoord5,
        MultiTexCoord6,
        MultiTexCoord7,                                        
    };
}

//-----------------------------------//

/**
 * Represents a vertex buffer.  One limitation here is that all data is 
 * tied to the vertex so if you want a normal per primitive and not per 
 * vertex you will have to duplicate that normal for each vertex for now.
 */

class VAPOR_API VertexBuffer : public Buffer
{
public:

    // Note: calls glGenBuffers (Base class Buffer, could do this)
	VertexBuffer();

    // Note: calls glDeleteBuffers (Base class Buffer, could do this)
    virtual ~VertexBuffer();

    // Updates the internal VBO with current values for vertices, 
    // normals, colors and texture coords.  Returns false on error, true otherwise.
    // Note: calls glBufferData
	bool build( BufferUsage::Enum = BufferUsage::Static,
				BufferAccess::Enum = BufferAccess::Write );

	// Returns true if the vertex buffer is built, false otherwhise.
	bool isBuilt() const;

	// Forces a rebuild of the vertex buffer the next update.
	void forceRebuild();
    
    // This method will make the internal VBO id bound so any future
    // glDrawXXX calls will use this VBO as its data.  
	// Returns false on error, true otherwise.
    // Note: calls glBindBuffer, glVertexAttribPointer, glEnableVertexAttribArray
    bool bind();
    
    // Puts opengl back into immediate mode
    // Note: calls glBindBuffer( 0 ), glDisableVertexAttribArray 
    bool unbind();
    
	// Returns if the this buffer is valid.
	bool isValid();

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
	uint getNumVertices();

	// These are all the possible types supported by glVertexAttrib
	//bool set( VertexAttribute::Enum attr, std::vector< byte > const& data );
	//bool set( VertexAttribute::Enum attr, std::vector< ubyte > const& data );
	//bool set( VertexAttribute::Enum attr, std::vector< short > const& data );
	//bool set( VertexAttribute::Enum attr, std::vector< ushort > const& data );
	//    
	//bool set( VertexAttribute::Enum attr, std::vector< int > const& data );
	//bool set( VertexAttribute::Enum attr, std::vector< uint > const& data );
	//bool set( VertexAttribute::Enum attr, std::vector< float > const& data );
	//bool set( VertexAttribute::Enum attr, std::vector< double > const& data );

	//bool set( VertexAttribute::Enum attr, std::vector< Vector2< byte > > const& data );
	//bool set( VertexAttribute::Enum attr, std::vector< Vector2< ubyte > > const& data );
	//bool set( VertexAttribute::Enum attr, std::vector< Vector2< short > > const& data );
	//bool set( VertexAttribute::Enum attr, std::vector< Vector2< ushort > > const& data );
	//bool set( VertexAttribute::Enum attr, std::vector< Vector2< int > > const& data );
	//bool set( VertexAttribute::Enum attr, std::vector< Vector2< uint > > const& data );
	//bool set( VertexAttribute::Enum attr, std::vector< Vector2< float > > const& data );
	//bool set( VertexAttribute::Enum attr, std::vector< Vector2< double > > const& data );
	//    	
	//bool set( VertexAttribute::Enum attr, std::vector< Vector3< byte >> const& data );
	//bool set( VertexAttribute::Enum attr, std::vector< Vector3< ubyte >> const& data );
	//bool set( VertexAttribute::Enum attr, std::vector< Vector3< short >> const& data );
	//bool set( VertexAttribute::Enum attr, std::vector< Vector3< ushort >> const& data );
	//bool set( VertexAttribute::Enum attr, std::vector< Vector3< int >> const& data );
	//bool set( VertexAttribute::Enum attr, std::vector< Vector3< uint >> const& data );
	bool set( VertexAttribute::Enum attr, const std::vector< Vector3 >& data );
	//bool set( VertexAttribute::Enum attr, std::vector< Vector3< float >> const& data );
	//bool set( VertexAttribute::Enum attr, std::vector< Vector3< double >> const& data );

	//bool set( VertexAttribute::Enum attr, std::vector< Vector4< byte > > const& data );
	//bool set( VertexAttribute::Enum attr, std::vector< Vector4< ubyte > > const& data );
	//bool set( VertexAttribute::Enum attr, std::vector< Vector4< short > > const& data );
	//bool set( VertexAttribute::Enum attr, std::vector< Vector4< ushort > > const& data );
	//bool set( VertexAttribute::Enum attr, std::vector< Vector4< int > > const& data );
	//bool set( VertexAttribute::Enum attr, std::vector< Vector4< uint > > const& data );
	//bool set( VertexAttribute::Enum attr, std::vector< Vector4< float > > const& data );
	//bool set( VertexAttribute::Enum attr, std::vector< Vector4< double > > const& data );

private:

	// Checks that each entry in the map has the same size.
	bool checkSize();

	// Binds all the OpenGL pointers when the buffer is built.
	void bindPointers();

	// Tells us if this buffer has already been built.
    bool built;

    // OpenGL buffer modifiers.
    BufferUsage::Enum bufferUsage;
    BufferAccess::Enum bufferAccess;

	// Used to store specific GL types for each attribute.
	typedef std::tuple< int, GLPrimitive::Enum, std::vector< byte > > Attribute;
	typedef std::map< VertexAttribute::Enum, Attribute > AttributeMap;
	typedef std::pair< const VertexAttribute::Enum, Attribute > AttributeMapPair;
	
	// Holds all the vertex attributes.
	mutable AttributeMap attributeMap;

	// Holds the number of vertices inside.
	uint numVertices;
};

//-----------------------------------//

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( VertexBuffer );

//-----------------------------------//

} } // end namespaces