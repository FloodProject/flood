/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/resources/Resource.h"

namespace vapor { namespace resources {

//-----------------------------------//

/**
 * Possible shader types.
 */

namespace ShaderType
{
	enum Enum 
	{
		Vertex,
		Fragment,
		Geometry
	};

	const std::string getString(Enum e);
}

//-----------------------------------//

/**
`* Shader resource.
 */

class VAPOR_API Shader : public Resource
{
public:

	Shader();

	// Gets the shader type.
	IMPLEMENT_ACESSOR(Type, ShaderType::Enum, type)

	// Gets the shader text.
	IMPLEMENT_ACESSOR(Text, const std::string&, text)

	// Gets the compilation log.
	IMPLEMENT_GETTER(Log, const std::string&, log)

	// Creates and loads the shader.
	virtual bool create() = 0;

	// Compiles the shader.
	virtual bool compile() = 0;
	
	// Is this shader already compiled?
	bool isCompiled() const;

	// Did this shader throw compile errors?
	bool gotCompileErrors() const;

	// Gets the associated resource group.
	IMPLEMENT_GETTER(ResourceGroup, ResourceGroup::Enum, ResourceGroup::Shaders)

protected:

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

//-----------------------------------//

TYPEDEF_RESOURCE_POINTER_FROM_TYPE( Shader );

//-----------------------------------//

} } // end namespaces