/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

#ifdef VAPOR_RENDERER_OPENGL

#include "vapor/render/VertexBuffer.h"
#include "vapor/render/GL.h"

namespace vapor {
	namespace render {

//-----------------------------------//

enum VertexBuffer::GLPrimitive
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

//-----------------------------------//

VertexBuffer::VertexBuffer()
	: built( false ), numVertices( 0 ), 
	bufferUsage( BufferUsage::Static ), bufferAccess( BufferAccess::Read )
{

}

//-----------------------------------//

VertexBuffer::~VertexBuffer()
{
	clear();
}

//-----------------------------------//

bool VertexBuffer::bind()
{
	glBindBuffer( GL_ARRAY_BUFFER, id );

#ifdef VAPOR_DEBUG
	while( glGetError() != GL_NO_ERROR )
	{
		warn( "gl::buffers", "Error binding vertex buffer" );
		return false;
	}
#endif

	bindPointers();

	return true;
}

//-----------------------------------//

void VertexBuffer::bindPointers()
{
	if( !built ) return;

	int offset = 0;

	foreach( const attributePair& p, attributeMap )
	{
		int components = tr1::get< 0 >( p.second );
		GLPrimitive type = tr1::get< 1 >( p.second );
		const std::vector<byte>& vec = tr1::get< 2 >( p.second );

		glEnableVertexAttribArray( p.first );

#ifdef VAPOR_DEBUG
		while( glGetError() != GL_NO_ERROR )
		{
			warn( "gl::buffers", "Error enabling vertex attribute array" );
			return;
		}
#endif

		glVertexAttribPointer( p.first, components, type, GL_FALSE, 0, (void*) offset );

#ifdef VAPOR_DEBUG
		// check for errors
		while( glGetError() != GL_NO_ERROR )
		{
			warn( "gl::buffers", "Error binding pointers to buffer" );
			return;
		}
#endif

		offset += vec.size();
	}
}

//-----------------------------------//

bool VertexBuffer::unbind()
{
	glBindBuffer( GL_ARRAY_BUFFER, 0 );

#ifdef VAPOR_DEBUG
	while( glGetError() != GL_NO_ERROR )
	{
		warn( "gl::buffers", "Error unbinding vertex buffer (id: '%d')", id );	
		return false;
	}
#endif

	if( built )
	{
		foreach( const attributePair& p, attributeMap )
		{
			glDisableVertexAttribArray( p.first );
		}
	}

	return true;
}

//-----------------------------------//

bool VertexBuffer::set( VertexAttribute::Enum attr, 
		const std::vector< math::Vector3 >& data )
{
	built = false;

	// TODO: optimize this, copies twice...
	std::vector< byte > bytev( data.size() * sizeof( math::Vector3 ) );
	
	if( data.size() != 0)
		memcpy( &bytev[0], &data[0], bytev.size() );
	
	attributeMap[attr] = tr1::make_tuple( 3, VertexBuffer::FLOAT, bytev );

	return true;
}

//-----------------------------------//

bool VertexBuffer::build( BufferUsage::Enum bU, BufferAccess::Enum bA )
{
	bufferUsage = bU;
	bufferAccess = bA;

	bind();

	// check that all vertex attributes elements are the same size
	// else we have to return because it doesn't make sense.

	if( !checkSize() ) return false;

	// reserve space for all the elements
	glBufferData( GL_ARRAY_BUFFER, getSize(), nullptr, getGLBufferType( bU, bA ) );

	//debug( "buffer '%d' has size '%d'", id, getSize() );

#ifdef VAPOR_DEBUG
	while( glGetError() != GL_NO_ERROR )
	{
		warn( "gl::buffers", "Could not allocate storage for buffer" );
		return false;
	}
#endif

	int offset = 0;
	foreach( const attributePair& p, attributeMap )
	{
		const std::vector<byte>& vec = tr1::get< 2 >( p.second );
		glBufferSubData( GL_ARRAY_BUFFER, offset, vec.size(), &vec[0] );
		offset += vec.size();
	}

#ifdef VAPOR_DEBUG
	while( glGetError() != GL_NO_ERROR )
	{
		warn( "gl::buffers", "Could not buffer the data" );
		return false;
	}
#endif

	built = true;

	return true;
}

//-----------------------------------//

bool VertexBuffer::checkSize()
{
	// TODO: should we also check each attribute has the same type?

	if( attributeMap.empty() ) return false;

	int first = -1;
	
	foreach( const attributePair& p, attributeMap )
	{
		int size = tr1::get< 2 >( p.second ).size();

		if( first < 0 )
		{
			first = size;

			// Update the number of vertices.
			// Should be the same for every attribute.
			numVertices = size / 
				(tr1::get< 0 >( p.second ) *
				sizeof( tr1::get< 1 >( p.second ) ) );
		}
		else if( size != first )
		{
			return false;
		}
	}

	return true;
}

//-----------------------------------//

uint VertexBuffer::getSize() const
{
	uint totalBytes = 0;

#ifdef VAPOR_COMPILER_MSVC
	// MSVC is ghey...
	#pragma warning (disable: 4503)
#endif

	foreach( const attributePair& p, attributeMap )
	{
		totalBytes += tr1::get< 2 >( p.second ).size();
	}

	return totalBytes;
}

//-----------------------------------//

uint VertexBuffer::getNumAttributes() const
{
	return attributeMap.size();
}

//-----------------------------------//

uint VertexBuffer::getNumVertices() const
{
	return numVertices;
}

//-----------------------------------//

void VertexBuffer::clear()
{
	attributeMap.clear();
}

//-----------------------------------//

bool VertexBuffer::isBuilt() const
{
	return built;
}

//-----------------------------------//

} } // end namespaces

#endif