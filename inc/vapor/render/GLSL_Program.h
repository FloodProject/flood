/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/render/Program.h"
#include "vapor/render/GLSL_Shader.h"

#include "vapor/render/GL.h"

namespace vapor {
	namespace render {

//-----------------------------------//
		
/**
 * GLSL Program.
 */

class VAPOR_API GLSL_Program : public Program
{
public:

	GLSL_Program( GLSL_ShaderPtr vs, GLSL_ShaderPtr ps );
	virtual ~GLSL_Program();

	// Adds a parameter to the shader.
	virtual void setAttribute( const std::string& slot, VertexAttribute::Enum attr );

	// Adds a uniform to the shader.
	virtual void setUniform( const std::string& slot, int data );

	// Adds a named matrix uniform to the program.
	virtual void setUniform( const std::string& slot, const math::Matrix4& );

	// Links the program.
	virtual bool link();

	// Binds the program.
	virtual void bind();

	// Unbinds the program.
	virtual void unbind();

	//uint id();

private:

	void bindDefaultAttributes();

	bool validate();

	void attachShaders();

	void getLogText();

	std::vector< GLSL_ShaderPtr > shaders;
	GLuint id;
};

//-----------------------------------//

} } // end namespaces