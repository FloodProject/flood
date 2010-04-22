/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#ifdef VAPOR_RENDERER_OPENGL

#define GLEW_STATIC
#include <GL/glew.h>

namespace vapor { namespace render {

//-----------------------------------//

// Handles OpenGL error states.
bool glHasError( const std::string& msg );

// Gets a string given an OpenGL error code.
// Adapted from the Mesa GLU implementation.
const char* glErrorString(GLenum errorCode);

//-----------------------------------//

} } // end namespaces

#endif