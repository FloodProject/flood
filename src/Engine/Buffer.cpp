/************************************************************************
*
* vaporEngine by triton © (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

#include "vapor/render/Buffer.h"
#include "vapor/render/GL.h"

namespace vapor {
	namespace render {
		
//-----------------------------------//		

Buffer::Buffer() 
{
	// TODO: make this check somewhere else
	if(GLEW_ARB_vertex_buffer_object)
	{
		glGenBuffers( 1, &id );

		if( glGetError() != GL_NO_ERROR )
		{
			warn( "gl::buffers", "Error generating a new buffer" );
			return;
		}

		debug( "created opengl buffer (id: '%d')", id );
	}
}

//-----------------------------------//

Buffer::~Buffer()
{
	glDeleteBuffers( 1, &id );

	if( glGetError() != GL_NO_ERROR )
	{
		warn( "gl::buffers", "Error deleting buffer (id: '%d')", id );
		return;
	}

	debug( "deleted opengl buffer (id: '%d')", id );
}

//-----------------------------------//

} } // end namespaces