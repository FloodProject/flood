/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Graphics/Shader.h"
#include "GL.h"

NAMESPACE_GRAPHICS_BEGIN

//-----------------------------------//

/**
 * GLSL implementation of shaders.
 */

class API_GRAPHICS GLSL_Shader : public Shader
{
public:

	GLSL_Shader();
	virtual ~GLSL_Shader();

	// Creates the shader.
	virtual bool create() OVERRIDE;

	// Compiles the shader.
	virtual bool compile() OVERRIDE;

	// Uploads the shader to OpenGL.
	bool upload();

	// Gets the GLSL compilation log.
	void getCompileLog();

	GLuint id;
	bool created;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( GLSL_Shader );

//-----------------------------------//

NAMESPACE_GRAPHICS_END