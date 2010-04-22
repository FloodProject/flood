/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"

#ifdef VAPOR_RENDERER_OPENGL

#include "vapor/render/VertexBuffer.h"
#include "vapor/render/GL.h"

namespace vapor { namespace render {

using namespace vapor::math;

//-----------------------------------//

enum VertexBuffer::GLPrimitive
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

//-----------------------------------//

VertexBuffer::VertexBuffer()
	: built( false ), numVertices( 0 ), 
	bufferUsage( BufferUsage::Static ),
	bufferAccess( BufferAccess::Read )
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

	if( glHasError( "Error binding vertex buffer" ) )
		return false;

	bindPointers();

	return true;
}

//-----------------------------------//

bool VertexBuffer::isValid()
{
	// Check that we have a valid buffer
	if( !glIsBuffer( id ) )
	{
		warn( "gl::buffers", "Vertex buffer is not valid" );
		return false;		
	}

	return true;
}

//-----------------------------------//

void VertexBuffer::bindPointers()
{
	if( !built ) return;

	int offset = 0;

	foreach( const AttributeMapPair& p, attributeMap )
	{
		int components = std::get< 0 >( p.second );
		GLPrimitive type = std::get< 1 >( p.second );
		const std::vector<byte>& vec = std::get< 2 >( p.second );

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
		foreach( const AttributeMapPair& p, attributeMap )
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
	
	attributeMap[attr] = std::make_tuple( 3, VertexBuffer::FLOAT, bytev );

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
	foreach( const AttributeMapPair& p, attributeMap )
	{
		const std::vector<byte>& vec = std::get< 2 >( p.second );
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
	
	foreach( const AttributeMapPair& p, attributeMap )
	{
		int size = std::get< 2 >( p.second ).size();

		if( size == 0 ) return false;

		if( first < 0 )
		{
			first = size;

			// Update the number of vertices.
			// Should be the same for every attribute.
			numVertices = size / 
				(std::get< 0 >( p.second ) *
				sizeof( std::get< 1 >( p.second ) ) );
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

	foreach( const AttributeMapPair& p, attributeMap )
	{
		totalBytes += std::get< 2 >( p.second ).size();
	}

	return totalBytes;
}

//-----------------------------------//

std::vector<Vector3>& VertexBuffer::getVertices() const
{
	return getAttribute( VertexAttribute::Position );
}

//-----------------------------------//

std::vector<Vector3>&
VertexBuffer::getAttribute( VertexAttribute::Enum attr ) const
{
	AttributeMap::iterator it;
	it = attributeMap.find( attr );

	if( it == attributeMap.end() )
		assert( "Can't return null reference" );
	
	Attribute& p = (*it).second;
	std::vector<byte>& arr = std::get<2>(p);

	return (std::vector<Vector3>&)(arr);
}

//-----------------------------------//

void VertexBuffer::forceRebuild()
{
	built = false;
}

//-----------------------------------//

uint VertexBuffer::getNumAttributes() const
{
	return attributeMap.size();
}

//-----------------------------------//

uint VertexBuffer::getNumVertices()
{
	checkSize();
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