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

namespace vapor { namespace render {

//-----------------------------------//

bool glHasError( const std::string& msg )
{
	bool err = false;

#ifdef VAPOR_DEBUG
	int glErr;
	while( (glErr = glGetError()) != GL_NO_ERROR )
	{
		warn( "gl", msg.c_str() );
		err = true;
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
};

const char* gluErrorString(GLenum errorCode)
{
    for (int i = 0; Errors[i].String; i++) 
	{
        if (Errors[i].Token == errorCode)
            return Errors[i].String;
    }

    return nullptr;
}

//-----------------------------------//

} } // end namespaces

#endif