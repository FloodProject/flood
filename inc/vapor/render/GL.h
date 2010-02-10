/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/CompileOptions.h"

#ifdef VAPOR_RENDERER_OPENGL

#define GLEW_STATIC
#include <GL/glew.h>

namespace vapor { namespace render {

//-----------------------------------//

// Handles OpenGL error states.
bool glHasError( const std::string& msg );

//-----------------------------------//

} } // end namespaces

#endif