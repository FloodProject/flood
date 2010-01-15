/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/CompileOptions.h"

#ifdef VAPOR_RENDERER_OPENGL
	#define GLEW_STATIC
	#include <GL/glew.h>
#elif
	#error "No OpenGL implementation"
#endif

