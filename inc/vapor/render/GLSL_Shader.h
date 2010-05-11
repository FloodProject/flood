/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/render/Shader.h"

namespace vapor { namespace render {

//-----------------------------------//

/**
 * GLSL Shader.
 */

class VAPOR_API GLSL_Shader : public Shader
{
public:

	GLSL_Shader();
	virtual ~GLSL_Shader();

	virtual bool create();

	// Compiles the shader.
	virtual bool compile();

	// Gets the OpenGL shader id.
	uint id();

private:

	bool upload();

	void getGLSLLog();
	uint getGLShaderType( ShaderType::Enum type );

	uint shaderId;
	bool created;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( GLSL_Shader );

//-----------------------------------//

} } // end namespaces