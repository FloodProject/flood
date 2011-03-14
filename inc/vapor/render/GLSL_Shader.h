/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Resources/Shader.h"

namespace vapor {

//-----------------------------------//

/**
 * GLSL Shader.
 */

class VAPOR_API GLSL_Shader : public Shader
{
	REFLECT_DECLARE_CLASS()

public:

	GLSL_Shader();
	virtual ~GLSL_Shader();

	// Creates the shader.
	virtual bool create();

	// Compiles the shader.
	virtual bool compile();

	// Uploads the shader to OpenGL.
	bool upload();

	// Gets the GLSL compilation log.
	void getCompileLog();

	// Returns the OpenGL shader type.
	uint32 getGLShaderType( ShaderType::Enum type );

	// Internal OpenGL shader id.
	uint32 id;

	// Was this shader already created?
	bool created;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( GLSL_Shader );

//-----------------------------------//

} // end namespace