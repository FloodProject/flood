/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"

#include "vapor/math/Vector3.h"
#include "vapor/render/Buffer.h"
#include "vapor/render/GL.h"

namespace vapor {
	namespace render {

//-----------------------------------//

/**
 * Attribute of a vertex element.
 * Matches the (NVIDIA-only?) specification
 */

namespace VertexAttribute
{
    enum Enum
    {
        Vertex = 0,
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
 * Represents a vertex buffer.  One limitation here is that all data is tied to the vertex
 * so if you want a normal per primitive and not per vertex you will have to duplicate
 * that normal for each vertex for now.
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
	bool build( BufferUsage::Enum bufferUsage = BufferUsage::Static, 
		BufferAccess::Enum bufferAccess = BufferAccess::Write );

	// Returns true if the vertex buffer is built, false otherwhise.
	bool isBuilt() const;
    
    // This method will make the internal VBO id bound so any future
    // glDrawXXX calls will use this VBO as its data.  
	// Returns false on error, true otherwise.
    // Note: calls glBindBuffer, glVertexAttribPointer, glEnableVertexAttribArray
    bool bind();
    
    // Puts opengl back into immediate mode
    // Note: calls glBindBuffer( 0 ), glDisableVertexAttribArray 
    bool unbind();
    
    // Clears this vertex buffer. All vertex data will be gone.
    void clear();

	// Returns the total size in bytes of the buffer.
	uint getSize() const;

	// Returns the number of vertex attributes.
	uint getNumAttributes() const;

	// Returns the number of vertices in each attribute.
	uint getNumVertices() const;

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

	//bool set( VertexAttribute::Enum attr, std::vector< math::Vector2< byte > > const& data );
	//bool set( VertexAttribute::Enum attr, std::vector< math::Vector2< ubyte > > const& data );
	//bool set( VertexAttribute::Enum attr, std::vector< math::Vector2< short > > const& data );
	//bool set( VertexAttribute::Enum attr, std::vector< math::Vector2< ushort > > const& data );
	//bool set( VertexAttribute::Enum attr, std::vector< math::Vector2< int > > const& data );
	//bool set( VertexAttribute::Enum attr, std::vector< math::Vector2< uint > > const& data );
	//bool set( VertexAttribute::Enum attr, std::vector< math::Vector2< float > > const& data );
	//bool set( VertexAttribute::Enum attr, std::vector< math::Vector2< double > > const& data );
	//    	
	//bool set( VertexAttribute::Enum attr, std::vector< math::Vector3< byte >> const& data );
	//bool set( VertexAttribute::Enum attr, std::vector< math::Vector3< ubyte >> const& data );
	//bool set( VertexAttribute::Enum attr, std::vector< math::Vector3< short >> const& data );
	//bool set( VertexAttribute::Enum attr, std::vector< math::Vector3< ushort >> const& data );
	//bool set( VertexAttribute::Enum attr, std::vector< math::Vector3< int >> const& data );
	//bool set( VertexAttribute::Enum attr, std::vector< math::Vector3< uint >> const& data );
	bool set( VertexAttribute::Enum attr, const std::vector< math::Vector3 >& data );
	//bool set( VertexAttribute::Enum attr, std::vector< math::Vector3< float >> const& data );
	//bool set( VertexAttribute::Enum attr, std::vector< math::Vector3< double >> const& data );

	//bool set( VertexAttribute::Enum attr, std::vector< math::Vector4< byte > > const& data );
	//bool set( VertexAttribute::Enum attr, std::vector< math::Vector4< ubyte > > const& data );
	//bool set( VertexAttribute::Enum attr, std::vector< math::Vector4< short > > const& data );
	//bool set( VertexAttribute::Enum attr, std::vector< math::Vector4< ushort > > const& data );
	//bool set( VertexAttribute::Enum attr, std::vector< math::Vector4< int > > const& data );
	//bool set( VertexAttribute::Enum attr, std::vector< math::Vector4< uint > > const& data );
	//bool set( VertexAttribute::Enum attr, std::vector< math::Vector4< float > > const& data );
	//bool set( VertexAttribute::Enum attr, std::vector< math::Vector4< double > > const& data );

private:

	// Used to store specific GL types for each attribute.
	enum GLPrimitive;

	// Checks that each entry in the map has the same size.
	bool checkSize();

	// Binds all the OpenGL pointers when the buffer is built.
	void bindPointers();

	// Tells us if this buffer has already been built.
    bool built;

    // OpenGL buffer modifiers
    BufferUsage::Enum bufferUsage;
    BufferAccess::Enum bufferAccess;
	
	typedef std::tuple< int, GLPrimitive, std::vector< byte > > attributeValue;
	typedef std::pair< const VertexAttribute::Enum, attributeValue > attributePair;

	// Holds all the vertex attributes
	std::map< VertexAttribute::Enum, attributeValue > attributeMap;

	uint numVertices;
};

//-----------------------------------//

TYPEDEF_SHARED_POINTER_FROM_CLASS( VertexBuffer );

//-----------------------------------//

} } // end namespaces