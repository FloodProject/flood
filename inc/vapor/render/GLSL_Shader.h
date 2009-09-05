/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/resources/Shader.h"

#include "vapor/render/GL.h"

namespace vapor {
	namespace render {

//-----------------------------------//

/**
 * GLSL Shader.
 */

class GLSL_Shader : public resources::Shader
{
public:

	GLSL_Shader( resources::ShaderType::Enum e, const std::string& text );
	virtual ~GLSL_Shader();

	// Compiles the shader.
	virtual bool compile();

	// Gets the OpenGL shader id.
	uint getID();

private:

	bool upload();

	void getGLSLLog();

	GLenum getGLShaderType( resources::ShaderType::Enum type );

	GLuint shaderId;
};

//-----------------------------------//

} } // end namespaces