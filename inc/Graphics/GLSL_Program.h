/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Render/Program.h"
#include "Render/VertexBuffer.h"
#include "Render/GLSL_ShaderProgram.h"
#include "Render/GL.h"

FWD_DECL_INTRUSIVE(GLSL_Shader)

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//
		
/**
 * GLSL Program.
 */

typedef std::vector< GLSL_ShaderProgram* > ShaderProgramVector;
typedef std::map< GLSL_ShaderProgram*, bool > ShaderProgramAttachMap;

class API_RENDER GLSL_Program : public Program
{
public:

	GLSL_Program();
	virtual ~GLSL_Program();

	// Creates the program.
	bool create() OVERRIDE;

	// Links the program.
	bool link() OVERRIDE;

	// Binds the program.
	void bind() OVERRIDE;

	// Unbinds the program.
	void unbind() OVERRIDE;

	// Creates the shaders and adds them to the program.
	void createShaders() OVERRIDE;

	// Adds a parameter to the shader.
	void setAttribute( const String&, VertexAttribute::Enum ) OVERRIDE;

	// Sets the uniforms in the program.
	void setUniforms( const UniformBufferPtr& ) OVERRIDE;

	// Gets the shader programs.
	GETTER(ShaderPrograms, const ShaderProgramVector&, shaders)

	// Forces the recompilation of all shader programs.
	void forceRecompile();

protected:

	// Adds a shader program to the program.
	void addShaderProgram( GLSL_ShaderProgram* );

	// Attaches all shaders to the program.
	bool compileShaderPrograms();

	// Detaches all attached shader programs.
	void detachShaderPrograms();

	// Binds the default engine attributes to the program.
	void bindDefaultAttributes();

	// Validates that the program is well-formed.
	bool validate();

	// Gets the linking log of the program.
	void getLogText();

	GLuint id;
	bool hadLinkError;

	ShaderProgramVector shaders;
	ShaderProgramAttachMap attached;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( GLSL_Program );

//-----------------------------------//

NAMESPACE_ENGINE_END