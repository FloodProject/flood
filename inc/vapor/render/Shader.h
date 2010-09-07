/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/resources/Resource.h"

namespace vapor {

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

	static const std::string getString(Enum e);
};

//-----------------------------------//

/**
 * Shaders are used to program the GPU programmable rendering pipeline.
 * It is a set of software instructions that instruct how to calculate
 * each vertex and each pixel processed by the graphics card. We only
 * deal with high-level shaders in the engine, which are compiled to
 * low-level assembly code by the graphics driver.
 */

class VAPOR_API Shader : public ReferenceCounted
{
public:

	// Gets the shader type.
	ACESSOR(Type, ShaderType::Enum, type)

	// Gets the shader text.
	ACESSOR(Text, const std::string&, text)

	// Gets the compilation log.
	GETTER(Log, const std::string&, log)

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

protected:

	Shader();

	// Shader type.
	ShaderType::Enum type;

	// Holds the shader text.
	std::string text;

	// Holds the compilation log.
	std::string log;
	
	// Is this shader already compiled?	
	bool compiled;
	bool compileErrors;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Shader );

//-----------------------------------//

} // end namespace