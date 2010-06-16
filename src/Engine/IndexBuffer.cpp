/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"

#ifdef VAPOR_RENDERER_OPENGL

#include "vapor/render/IndexBuffer.h"
#include "vapor/render/GL.h"

namespace vapor {

//-----------------------------------//

IndexBuffer::IndexBuffer( IndexBufferType::Enum indexType )
	: built( false ), indexType( indexType )
{

}

//-----------------------------------//

void IndexBuffer::set( const std::vector< ushort >& data )
{
	data16 = data;
	built = false;
}

//-----------------------------------//

void IndexBuffer::set( const std::vector< ulong >& data )
{
	data32 = data;
	built = false;
}

//-----------------------------------//

bool IndexBuffer::bind()
{
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, id );

	if( glHasError( "Error binding index buffer" ) )
		return false;

	return true;
}

//-----------------------------------//

bool IndexBuffer::unbind()
{
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

	if( glHasError( "Error unbinding index buffer" ) )
		return false;

	return true;
}

//-----------------------------------//

bool IndexBuffer::build( BufferUsage::Enum bU, BufferAccess::Enum bA )
{
	bind();

	if( data16.empty() && data32.empty() ) return false;

	// reserve space for all the elements
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, 
		getNumIndices() * (is16bit() ? sizeof(ushort) : sizeof(ulong)), 
		is16bit() ? &data16[0] : reinterpret_cast<ushort*>(&data32[0]), 
		getGLBufferType( bU, bA ) );

	//debug( "index buffer '%d' has size '%d'", id, getNumIndices() );

	if( glHasError("Could not buffer data in index buffer") )
		return false;

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

} // end namespace

#endif
