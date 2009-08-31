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
 *   0 – gl_Vertex
 *   2 – gl_Normal
 *   3 – gl_Color
 *   4 – gl_SecondaryColor
 *   5 – gl_FogCoord
 *   8 – gl_MultiTexCoord0
 *   9 – gl_MultiTexCoord1
 *   10 – gl_MultiTexCoord2
 *   11 – gl_MultiTexCoord3
 *   12 – gl_MultiTexCoord4
 *   13 – gl_MultiTexCoord5
 *   14 – gl_MultiTexCoord6
 *   15 – gl_MultiTexCoord7
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

namespace GLPrimitiveType
{
    enum Enum
    {
        BYTE = GL_BYTE,
        UBYTE = GL_UNSIGNED_BYTE,
        SHORT = GL_SHORT,
        USHORT = GL_UNSIGNED_SHORT,
        INT = GL_INT,
        UINT = GL_UNSIGNED_INT,
        FLOAT = GL_FLOAT,
        DOUBLE = GL_DOUBLE
    };
}

//-----------------------------------//

/**
 * Represents a vertex buffer.  One limitation here is that all data is tied to the vertex
 * so if you want a normal per primitive and not per vertex you will have to duplicate
 * that normal for each vertex for now.
 */

class VertexBuffer : public Buffer
{
public:

    // Note: calls glGenBuffers (Base class Buffer, could do this)
	VertexBuffer();

    // Note: calls glDeleteBuffers (Base class Buffer, could do this)
    virtual ~VertexBuffer();

    // Updates the internal VBO with current values for vertices, 
    // normals, colors and texture coords.  Returns false on error, true otherwise.
    // Note: calls glBufferData
    bool build( BufferUsage::Enum bufferUsage, BufferAccess::Enum bufferAccess );
    
    // This method will make the internal VBO id bound so any future
    // glDrawXXX calls will use this VBO as its data.  Returns false on error, true
    // otherwise.
    // Note: calls glBindBuffer, glVertexAttribPointer, glEnableVertexAttribArray ,glEnableClientState
    bool bind();
    
    // Puts opengl back into immediate mode
    // Note: calls glBindBuffer( 0 ), glDisableVertexAttribArray 
    bool unbind();
    
    // Clears this vertex buffer. All vertex data will be gone.
    void clear();

	bool set( VertexAttribute::Enum attr, const std::vector< math::Vector3 >& data );

 //   // these are all the possible types supported by glVertexAttrib
 //   bool set( VertexAttribute::Enum attr, std::vector< byte > const& data );
 //   //bool set( VertexAttribute::Enum attr, std::vector< ubyte > const& data );
 //   bool set( VertexAttribute::Enum attr, std::vector< short > const& data );
 //   bool set( VertexAttribute::Enum attr, std::vector< ushort > const& data );
 //   //{
 //   //    std::vector< byte > bytev( data.size() * sizeof( ushort ) );
 //   //    memcpy( &bytev[0], &data[0], bytev.size() );
 //   //    datamap[attr] = tr1::tuple( 1, PrimitiveType::USHORT, bytev );
 //   //}
 //   //    
 //   bool set( VertexAttribute::Enum attr, std::vector< int > const& data );
 //   bool set( VertexAttribute::Enum attr, std::vector< uint > const& data );
 //   bool set( VertexAttribute::Enum attr, std::vector< float > const& data );
 //   bool set( VertexAttribute::Enum attr, std::vector< double > const& data );
 //   
 //   // VectorX overloads do not need numComponents since the template parameter
 //   // will tell us that, for example math::Vector3 means 3 components
 //   
 //   //bool set( VertexAttribute::Enum attr, std::vector< math::Vector2< byte > > const& data );
 //   ////bool set( VertexAttribute::Enum attr, std::vector< math::Vector2< ubyte > > const& data );
 //   //bool set( VertexAttribute::Enum attr, std::vector< math::Vector2< short > > const& data );
 //   //bool set( VertexAttribute::Enum attr, std::vector< math::Vector2< ushort > > const& data );
 //   //bool set( VertexAttribute::Enum attr, std::vector< math::Vector2< int > > const& data );
 //   //bool set( VertexAttribute::Enum attr, std::vector< math::Vector2< uint > > const& data );
 //   //bool set( VertexAttribute::Enum attr, std::vector< math::Vector2< float > > const& data );
 //   //bool set( VertexAttribute::Enum attr, std::vector< math::Vector2< double > > const& data );
 //       
 //   bool set( VertexAttribute::Enum attr, std::vector< math::Vector3< byte >> const& data );
 //   bool set( VertexAttribute::Enum attr, std::vector< math::Vector3< ubyte >> const& data );
 //   bool set( VertexAttribute::Enum attr, std::vector< math::Vector3< short >> const& data );
 //   bool set( VertexAttribute::Enum attr, std::vector< math::Vector3< ushort >> const& data );
 //   bool set( VertexAttribute::Enum attr, std::vector< math::Vector3< int >> const& data );
 //   bool set( VertexAttribute::Enum attr, std::vector< math::Vector3< uint >> const& data );
 //   bool set( VertexAttribute::Enum attr, std::vector< math::Vector3< float >> const& data );
 //   bool set( VertexAttribute::Enum attr, std::vector< math::Vector3< double >> const& data );

 //   //bool set( VertexAttribute::Enum attr, std::vector< math::Vector4< byte > > const& data );
 //   ////bool set( VertexAttribute::Enum attr, std::vector< math::Vector4< ubyte > > const& data );
 //   //bool set( VertexAttribute::Enum attr, std::vector< math::Vector4< short > > const& data );
 //   //bool set( VertexAttribute::Enum attr, std::vector< math::Vector4< ushort > > const& data );
 //   //bool set( VertexAttribute::Enum attr, std::vector< math::Vector4< int > > const& data );
 //   //bool set( VertexAttribute::Enum attr, std::vector< math::Vector4< uint > > const& data );
 //   //bool set( VertexAttribute::Enum attr, std::vector< math::Vector4< float > > const& data );
 //   //bool set( VertexAttribute::Enum attr, std::vector< math::Vector4< double > > const& data )
 //   //{
 //   //    std::vector< byte > bytev( data.size() * sizeof( math::Vector4< double >) );
 //   //    memcpy( &bytev[0], &data[0], bytev.size() );
 //   //    datamap[attr] = std::tuple( 4, PrimitiveType::DOUBLE, bytev );
 //   }

private:

    bool built;
    
    // OpenGL buffer modifiers
    BufferUsage::Enum bufferUsage;
    BufferAccess::Enum bufferAccess;

	GLenum getGLBufferType( BufferUsage::Enum bU, BufferAccess::Enum bA );
	
	typedef tr1::tuple< int, GLPrimitiveType::Enum, std::vector< byte > > datakey;
	typedef std::pair< const VertexAttribute::Enum, datakey > datapair;

	std::map< VertexAttribute::Enum, datakey > datamap;
};

//-----------------------------------//

} } // end namespaces