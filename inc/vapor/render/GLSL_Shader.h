/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "render/Shader.h"

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
	GETTER(Id, uint, id)

private:

	// Uploads the shader to OpenGL.
	bool upload();

	// Gets the GLSL compilation log.
	void getCompileLog();

	// Returns the OpenGL shader type.
	uint getGLShaderType( ShaderType::Enum type );

	// Internal OpenGL shader id.
	uint id;

	// Was this shader already created?
	bool created;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( GLSL_Shader );

//-----------------------------------//

} // end namespace