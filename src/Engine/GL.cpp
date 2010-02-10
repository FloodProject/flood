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
	while( glGetError() != GL_NO_ERROR )
	{
		warn( "gl", msg.c_str() );
		err = true;
	}
#endif

	return err;
}

//-----------------------------------//

} } // end namespaces

#endif