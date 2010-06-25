/************************************************************************
*
* vaporEngine by triton © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/render/Buffer.h"
#include "vapor/render/GL.h"

namespace vapor {
		
//-----------------------------------//		

Buffer::Buffer() 
{
	// TODO: make this check somewhere else
	if(GLEW_ARB_vertex_buffer_object)
	{
		glGenBuffers( 1, &id );

		if( glHasError("Error generating a new buffer") )
			return;
	}
}

//-----------------------------------//

Buffer::~Buffer()
{
	glDeleteBuffers( 1, &id );

	if( glHasError("Error deleting buffer") )
		return;
}

//-----------------------------------//

GLenum Buffer::getGLBufferType( BufferUsage::Enum bU, BufferAccess::Enum bA )
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

} // end namespace