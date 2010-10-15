/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"

#ifdef VAPOR_RENDERER_OPENGL

#include "vapor/render/GL.h"

namespace vapor {

//-----------------------------------//

bool glHasError( const std::string& msg )
{
	bool err = false;
	uint i = 5;

#ifdef VAPOR_DEBUG
	int gl_err;
	while( (gl_err = glGetError()) != GL_NO_ERROR )
	{
		Log::warn( msg.c_str() );
		err = true;

		if( --i == 0 )
			break;
	}
#endif

	return err;
}

//-----------------------------------//

struct token_string
{
   GLuint Token;
   const char *String;
};

static const struct token_string Errors[] =
{
	{ GL_NO_ERROR, "No error" },
	{ GL_INVALID_ENUM, "Invalid enumerant" },
	{ GL_INVALID_VALUE, "Invalid value" },
	{ GL_INVALID_OPERATION, "Invalid operation" },
	{ GL_STACK_OVERFLOW, "Stack overflow" },
	{ GL_STACK_UNDERFLOW, "Stack underflow" },
	{ GL_OUT_OF_MEMORY, "Out of memory" },
	{ GL_TABLE_TOO_LARGE, "Table too large" },
	{ GL_INVALID_FRAMEBUFFER_OPERATION_EXT, "Invalid framebuffer operation" },
	{ GL_FRAMEBUFFER_COMPLETE_EXT, "Framebuffer complete" },
	{ GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT, "Framebuffer has incomplete attachment" },
	{ GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT, "Framebuffer is missing attachment" },
	{ GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT, "Framebuffer has incomplete dimensions" },
	{ GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT, "Framebuffer has incomplete formats" },
	{ GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT, "Framebuffer has incomplete draw buffer" },
	{ GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT, "Framebuffer has incomplete read buffer" },
	{ GL_FRAMEBUFFER_UNSUPPORTED_EXT, "Framebuffer" },
};

const char* glErrorString(GLenum errorCode)
{
    for (int i = 0; Errors[i].String; i++) 
	{
        if (Errors[i].Token == errorCode)
            return Errors[i].String;
    }

    return nullptr;
}

//-----------------------------------//

} // end namespace

#endif