/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/render/Shader.h"

namespace vapor {

//-----------------------------------//

/**
 * GLSL Shader.
 */

class VAPOR_API GLSL_Shader : public Shader
{
public:

	GLSL_Shader();
	virtual ~GLSL_Shader();

	// Creates the shader.
	virtual bool create();

	// Compiles the shader.
	virtual bool compile();

	// Gets the OpenGL shader id.
	uint id();

private:

	// Uploads the shader to OpenGL.
	bool upload();

	// Gets the GLSL compilation log.
	void getGLSLLog();

	// Returns the OpenGL shader type.
	uint getGLShaderType( ShaderType::Enum type );

	// Internal OpenGL shader id.
	uint shaderId;

	// Was this shader already created?
	bool created;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( GLSL_Shader );

//-----------------------------------//

} // end namespace