/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/render/Program.h"
#include "vapor/render/GLSL_Shader.h"
#include "vapor/render/GL.h"

namespace vapor { namespace render {

//-----------------------------------//
		
/**
 * GLSL Program.
 */

class VAPOR_API GLSL_Program : public Program
{
public:

	GLSL_Program( const GLSL_ShaderPtr& vs, const GLSL_ShaderPtr& ps );
	virtual ~GLSL_Program();

	// Creates the program.
	virtual bool create();

	// Adds a parameter to the shader.
	virtual void setAttribute( const std::string& slot, VertexAttribute::Enum attr );

	// Adds a uniform to the shader.
	virtual void setUniform( const std::string& slot, int data );

	// Adds a named float array uniform to the program.
	//virtual void setUniform( const std::string& slot, const std::vector<float> vec ) = 0;

	// Adds a named Vector3 array uniform to the program.
	virtual void setUniform( const std::string& slot, const std::vector<math::Vector3> vec );

	// Adds a named color array uniform to the program.
	virtual void setUniform( const std::string& slot, const std::vector<math::Color> vec );

	// Adds a named vector uniform to the program.
	virtual void setUniform( const std::string& slot, const math::Vector3& vec );

	// Adds a named matrix uniform to the program.
	virtual void setUniform( const std::string& slot, const math::Matrix4x3& );

	// Adds a named 4x4 matrix uniform to the program.
	virtual void setUniform( const std::string& slot, const math::Matrix4x4& );

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

	bool attachShaders();

	void getLogText();

	bool linkError;
	std::vector< GLSL_ShaderPtr > shaders;
	GLuint id;
};

//-----------------------------------//

} } // end namespaces