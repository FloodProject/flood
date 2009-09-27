/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

#ifdef VAPOR_RENDERER_OPENGL

#include "vapor/render/IndexBuffer.h"
#include "vapor/render/GL.h"

namespace vapor {
	namespace render {

//-----------------------------------//

IndexBuffer::IndexBuffer( IndexBufferType::Enum indexType )
	: built( false ), indexType( indexType )
{

}

//-----------------------------------//

IndexBuffer::~IndexBuffer()
{
	//clear();
}

//-----------------------------------//

void IndexBuffer::set( const std::vector< ushort >& data )
{
	data16 = data;
}

//-----------------------------------//

void IndexBuffer::set( const std::vector< ulong >& data )
{
	data32 = data;
}

//-----------------------------------//

bool IndexBuffer::bind()
{
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, id );

#ifdef VAPOR_DEBUG
	if( glGetError() != GL_NO_ERROR )
	{
		warn( "gl::buffers", "Error binding index buffer" );
		return false;
	}
#endif

	return true;
}

//-----------------------------------//

bool IndexBuffer::unbind()
{
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

#ifdef VAPOR_DEBUG
	if( glGetError() != GL_NO_ERROR )
	{
		warn( "gl::buffers", "Error unbinding index buffer" );
		return false;
	}
#endif

	return true;
}

//-----------------------------------//

bool IndexBuffer::build( BufferUsage::Enum bU, BufferAccess::Enum bA )
{
	bind();

	// reserve space for all the elements
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, 
		getNumIndices() * (is16bit() ? sizeof(ushort) : sizeof(ulong)), 
		is16bit() ? &data16[0] : reinterpret_cast<ushort*>(&data32[0]), 
		getGLBufferType( bU, bA ) );

	debug( "index buffer '%d' has size '%d'", id, getNumIndices() );

#ifdef VAPOR_DEBUG
	if( glGetError() != GL_NO_ERROR )
	{
		warn( "gl::buffers", "Could not buffer data in index buffer" );
		return false;
	}
#endif

	built = true;
	return true;
}

//-----------------------------------//

uint IndexBuffer::getNumIndices() const
{
	if( is16bit() )
		return data16.size();
	else
		return data32.size();
}

//-----------------------------------//

bool IndexBuffer::isBuilt() const
{
	return built;
}

//-----------------------------------//

bool IndexBuffer::is16bit() const
{
	return ( indexType == IndexBufferType::I16bit );
}

//-----------------------------------//

} } // end namespaces

#endif