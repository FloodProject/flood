/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Graphics/ShaderProgram.h"
#include "Graphics/VertexBuffer.h"
#include "GL.h"

FWD_DECL_INTRUSIVE(GLSL_Shader)

NAMESPACE_GRAPHICS_BEGIN

//-----------------------------------//

/**
 * GLSL implementation of shader programs.
 */

typedef std::vector< GLSL_ShaderPtr > ShadersVector;
typedef std::map< GLSL_ShaderPtr, bool > ShadersAttachMap;

class API_RENDER GLSL_ShaderProgram : public ShaderProgram
{
public:

	GLSL_ShaderProgram();
	virtual ~GLSL_ShaderProgram();

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
	void setUniforms( UniformBuffer* ) OVERRIDE;

	// Gets the shaders in the program.
	GETTER(Shaders, const ShadersVector&, shaders)

	// Forces the recompilation of all shader programs.
	void forceRecompile();

protected:

	// Adds a shader program to the program.
	void addShader( GLSL_Shader* );

	// Attaches all shaders to the program.
	bool compileShaders();

	// Detaches all attached shader programs.
	void detachShaders();

	// Binds the default engine attributes to the program.
	void bindDefaultAttributes();

	// Validates that the program is well-formed.
	bool validate();

	// Gets the linking log of the program.
	void getLogText();

public:

	GLuint id;
	bool hadLinkError;
	ShadersVector shaders;
	ShadersAttachMap attached;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( GLSL_ShaderProgram );

//-----------------------------------//

NAMESPACE_GRAPHICS_END