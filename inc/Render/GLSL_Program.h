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
#include "Render/GLSL_Shader.h"
#include "Render/GL.h"

FWD_DECL_INTRUSIVE(GLSL_Text)

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//
		
/**
 * GLSL Program.
 */

class API_RENDER GLSL_Program : public Program
{
public:

	GLSL_Program( const GLSL_TextPtr& );
	virtual ~GLSL_Program();

	// Creates the program.
	virtual bool create() OVERRIDE;

	// Links the program.
	virtual bool link() OVERRIDE;

	// Binds the program.
	virtual void bind() OVERRIDE;

	// Unbinds the program.
	virtual void unbind() OVERRIDE;

	// Creates the shaders and adds them to the program.
	virtual void createShaders() OVERRIDE;

	// Updates the shader's text with the program text.
	virtual void updateShadersText() OVERRIDE;

	// Gets/sets the resource text that backs this shader.
	ACESSOR(Text, const GLSL_TextPtr&, text)

	// Adds a parameter to the shader.
	virtual void setAttribute( const String& slot, VertexAttribute::Enum attr ) OVERRIDE;

	// Sets the uniforms in the program.
	virtual void setUniforms( const UniformBufferPtr& ub ) OVERRIDE;

protected:

	// Adds a shader to the program.
	void addShader( const GLSL_ShaderPtr& );

	// Binds the default engine attributes to the program.
	void bindDefaultAttributes();

	// Validates that the program is well-formed.
	bool validate();

	// Compiles (if needed) and attaches all shaders to the program.
	bool attachShaders();

	// Gets the linking log of the program.
	void getLogText();

	GLuint id;
	bool linkError;
	GLSL_TextPtr text;

	typedef std::vector< GLSL_ShaderPtr > ShaderVector;
	ShaderVector shaders;

	typedef std::map< GLSL_ShaderPtr, bool > ShaderAttachMap;
	ShaderAttachMap attached;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( GLSL_Program );

//-----------------------------------//

NAMESPACE_ENGINE_END