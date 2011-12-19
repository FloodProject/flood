/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Resources/Shader.h"
#include "Resources/ShaderProgram.h"

NAMESPACE_GRAPHICS_BEGIN

//-----------------------------------//

/**
 * GLSL Shader.
 */

REFLECT_DECLARE_CLASS(GLSL_ShaderProgram)

class API_GRAPHICS GLSL_ShaderProgram : public ShaderProgram
{
	REFLECT_DECLARE_OBJECT(GLSL_ShaderProgram)

public:

	GLSL_ShaderProgram();
	virtual ~GLSL_ShaderProgram();

	// Creates the shader.
	virtual bool create();

	// Compiles the shader.
	virtual bool compile();

	// Uploads the shader to OpenGL.
	bool upload();

	// Gets the GLSL compilation log.
	void getCompileLog();

	// Was this shader already created?
	bool created;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( GLSL_ShaderProgram );

//-----------------------------------//

NAMESPACE_GRAPHICS_END