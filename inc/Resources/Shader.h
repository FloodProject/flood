/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Resources/Text.h"

NAMESPACE_RESOURCES_BEGIN

//-----------------------------------//

/**
 * Shaders can be of different types, depending on what they deal with.
 *  - Vertex shaders manipulate vertex properties such as position.
 *  - Pixel shaders calculate the color of individual pixels (fragments).
 *  - Geometry shaders can be used to generate geometry procedurally.
 */

struct ShaderType
{
	enum Enum 
	{
		Vertex,
		Fragment,
		Geometry
	};
};

REFLECT_DECLARE_ENUM(ShaderType)

//-----------------------------------//

/**
 * Shaders are used to program the GPU programmable rendering pipeline.
 * It is a set of software instructions that instruct how to calculate
 * each vertex and each pixel processed by the graphics card. We only
 * deal with high-level shaders in the engine, which are compiled to
 * low-level assembly code by the graphics driver.
 */

REFLECT_DECLARE_CLASS(Shader)

class API_RESOURCE Shader : public Resource
{
	REFLECT_DECLARE_OBJECT(Shader)

public:

	// Gets the shader type.
	ACESSOR(ShaderType, ShaderType::Enum, type)

	// Gets the shader text.
	ACESSOR(Text, const String&, text)

	// Gets the compilation log.
	GETTER(Log, const String&, log)

	// Creates and loads the shader.
	virtual bool create() = 0;

	// Compiles the shader.
	virtual bool compile() = 0;

	// Forces the shader to be recompiled.
	virtual void forceRecompile();
	
	// Is this shader already compiled?
	bool isCompiled() const;

	// Did this shader throw compile errors?
	bool gotCompileErrors() const;

	// Return the proper resource group for this resource.
	GETTER(ResourceGroup, ResourceGroup::Enum, ResourceGroup::Shaders)

	// Shader type.
	ShaderType::Enum type;

	// Holds the shader text.
	String text;

	// Holds the compilation log.
	String log;
	
	// Is this shader already compiled?	
	bool compiled;
	bool compileErrors;

protected:

	Shader();
};

TYPEDEF_RESOURCE_HANDLE_FROM_TYPE( Shader );
TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Shader );

//-----------------------------------//

NAMESPACE_RESOURCES_END