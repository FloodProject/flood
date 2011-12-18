/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"

#ifdef ENABLE_RENDERER_OPENGL

#include "Graphics/GL.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

bool CheckLastErrorGL( const char* msg )
{
#ifdef BUILD_DEBUG_
	bool occured = false;
	uint numMaxErrors = 5;

	GLenum error = 0;
	while(((error = glGetError()) != GL_NO_ERROR) && (numMaxErrors-- > 0) )
	{
		LogAssert("OpenGL: %s", msg);
		occured = true;
	}

	return occured;
#else
	return false;
#endif
}

//-----------------------------------//

#ifdef DEBUG_BUILD

struct TokenString
{
   GLuint token;
   const char *string;
};

static const struct TokenString errors[] =
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
    for (int i = 0; errors[i].string; i++) 
	{
        if(errors[i].token == errorCode)
            return errors[i].string;
    }

    return nullptr;
}

#endif

//-----------------------------------//

NAMESPACE_ENGINE_END

#endif