/************************************************************************
*
* vaporEngine by triton © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "render/Buffer.h"
#include "render/GL.h"

namespace vapor {
		
//-----------------------------------//		

Buffer::Buffer() 
	: usage(BufferUsage::Static)
	, access(BufferAccess::Read)
{
	glGenBuffers( 1, &id );

	if( glHasError("Error generating a new buffer") )
		return;
}

//-----------------------------------//		

Buffer::Buffer(BufferUsage::Enum usage, BufferAccess::Enum access) 
	: usage(usage)
	, access(access)
{
	glGenBuffers( 1, &id );

	if( glHasError("Error generating a new buffer") )
		return;
}

//-----------------------------------//

Buffer::~Buffer()
{
	glDeleteBuffers( 1, &id );

	if( glHasError("Error deleting buffer") )
		return;
}

//-----------------------------------//

int Buffer::getGLBufferType()
{
	if( usage == BufferUsage::Stream && access == BufferAccess::Read )
		return GL_STREAM_READ;
	
	if( usage == BufferUsage::Stream && access == BufferAccess::Write )
		return GL_STREAM_DRAW;
	
	if( usage == BufferUsage::Stream && access == BufferAccess::ReadWrite )
		return GL_STREAM_COPY;

	if( usage == BufferUsage::Static && access == BufferAccess::Read )
		return GL_STATIC_READ;
	
	if( usage == BufferUsage::Static && access == BufferAccess::Write )
		return GL_STATIC_DRAW;
	
	if( usage == BufferUsage::Static && access == BufferAccess::ReadWrite )
		return GL_STATIC_COPY;

	if( usage == BufferUsage::Dynamic && access == BufferAccess::Read )
		return GL_DYNAMIC_READ;
	
	if( usage == BufferUsage::Dynamic && access == BufferAccess::Write )
		return GL_DYNAMIC_DRAW;
	
	if( usage == BufferUsage::Dynamic && access == BufferAccess::ReadWrite )
		return GL_DYNAMIC_COPY;

	return GL_STREAM_READ;
}

//-----------------------------------//

} // end namespace