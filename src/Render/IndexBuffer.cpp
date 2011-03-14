/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"

#ifdef VAPOR_RENDERER_OPENGL

#include "Render/IndexBuffer.h"
#include "Render/GL.h"

namespace vapor {

//-----------------------------------//

IndexBuffer::IndexBuffer( int indexSize )
	: isBuilt(false)
	, indexSize(indexSize)
{ }

//-----------------------------------//

void IndexBuffer::setData( const std::vector<byte>& data )
{
	this->data = data;
	isBuilt = false;
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

bool IndexBuffer::build()
{
	if( data.empty() ) return false;

	bind();

	// Reserve space for all the buffer elements.
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, data.size(), &data[0], getGLBufferType() );

	if( glHasError("Could not buffer data in index buffer") )
		return false;

	isBuilt = true;
	return true;
}

//-----------------------------------//

int IndexBuffer::getSize() const
{
	return data.size() / (indexSize / 8);
}

//-----------------------------------//

} // end namespace

#endif
