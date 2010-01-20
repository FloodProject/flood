/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/resources/Shader.h"
#include "vapor/render/GL.h"

namespace vapor { namespace render {

//-----------------------------------//

/**
 * GLSL Shader.
 */

class VAPOR_API GLSL_Shader : public resources::Shader
{
public:

	GLSL_Shader( resources::ShaderType::Enum e, const std::string& text );
	virtual ~GLSL_Shader();

	// Compiles the shader.
	virtual bool compile();

	// Gets the OpenGL shader id.
	uint id();

private:

	bool upload();

	void getGLSLLog();

	GLenum getGLShaderType( resources::ShaderType::Enum type );

	GLuint shaderId;
};

//-----------------------------------//

TYPEDEF_RESOURCE_POINTER_FROM_TYPE( GLSL_Shader );

//-----------------------------------//

} } // end namespaces