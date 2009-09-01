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

VertexBuffer::VertexBuffer()
	: built( false )
{
	
}

//-----------------------------------//

VertexBuffer::~VertexBuffer()
{

}

//-----------------------------------//

bool VertexBuffer::bind()
{
	glBindBuffer( GL_ARRAY_BUFFER, id );

	if( glGetError() != GL_NO_ERROR )
	{
		warn( "gl::buffers", "Error binding vertex buffer" );
		return false;
	}

	debug( "bound vertex buffer (id: '%d')", id );	

	return true;
}

//-----------------------------------//

bool VertexBuffer::unbind()
{
	glBindBuffer( GL_ARRAY_BUFFER, 0 );

	if( glGetError() != GL_NO_ERROR )
	{
		warn( "gl::buffers", "Error unbinding vertex buffer (id: '%d')", id );	
		return false;
	}

	return true;
}

//-----------------------------------//

bool VertexBuffer::set( VertexAttribute::Enum attr, 
		const std::vector< math::Vector3 >& data )
{
	built = false;

	std::vector< byte > bytev( data.size() * sizeof( math::Vector3 ) );
	memcpy( &bytev[0], &data[0], bytev.size() );
	datamap[attr] = tr1::make_tuple( 3, GLPrimitiveType::FLOAT, bytev );

	return true;
}

//-----------------------------------//

bool VertexBuffer::build( BufferUsage::Enum bU, BufferAccess::Enum bA )
{
	bind();

	uint totalBytes = 0;

#ifdef VAPOR_COMPILER_MSVC
	// MSVC is ghey...
	#pragma warning (disable: 4503)
#endif

	foreach( const datapair& p, datamap )
	{
		totalBytes += tr1::get< 2 >( p.second ).size();
	}

	glBufferData( GL_ARRAY_BUFFER, totalBytes, nullptr, getGLBufferType( bU, bA ) );

	debug( "buffer '%d' has size '%d'", id, totalBytes );

	if( glGetError() != GL_NO_ERROR )
	{
		warn( "gl:buffers", "Could not allocate storage for buffer" );
		return false;
	}

	int offset = 0;
	foreach( const datapair& p, datamap )
	{
		const std::vector<byte>& vec = tr1::get< 2 >( p.second );
		glBufferSubData( GL_ARRAY_BUFFER, offset, vec.size(), &vec[0] );
		offset += vec.size();
	}

	if( glGetError() != GL_NO_ERROR )
	{
		warn( "gl:buffers", "Could not buffer the data" );
		return false;
	}

	size = totalBytes;
	built = true;

	return true;
}

//-----------------------------------//

void VertexBuffer::clear()
{
	datamap.clear();
}

//-----------------------------------//

GLenum VertexBuffer::getGLBufferType( BufferUsage::Enum bU, BufferAccess::Enum bA )
{
	// TODO: is this right? -.-

	if( bU == BufferUsage::Stream && bA == BufferAccess::Read ) return GL_STREAM_READ;
	if( bU == BufferUsage::Stream && bA == BufferAccess::Write ) return GL_STREAM_DRAW;
	if( bU == BufferUsage::Stream && bA == BufferAccess::ReadWrite ) return GL_STREAM_COPY;

	if( bU == BufferUsage::Static && bA == BufferAccess::Read ) return GL_STATIC_READ;
	if( bU == BufferUsage::Static && bA == BufferAccess::Write ) return GL_STATIC_DRAW;
	if( bU == BufferUsage::Static && bA == BufferAccess::ReadWrite ) return GL_STATIC_COPY;

	if( bU == BufferUsage::Dynamic && bA == BufferAccess::Read ) return GL_DYNAMIC_READ;
	if( bU == BufferUsage::Dynamic && bA == BufferAccess::Write ) return GL_DYNAMIC_DRAW;
	if( bU == BufferUsage::Dynamic && bA == BufferAccess::ReadWrite ) return GL_DYNAMIC_COPY;

	return GL_STREAM_READ;
}

//-----------------------------------//

} } // end namespaces

#endif