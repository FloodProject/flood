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

	Shader( ShaderType::Enum e, const std::string& text );
	virtual ~Shader();

	// Gets the shader type.
	ShaderType::Enum getType() const;

	// Gets the shader text.
	const std::string& getText() const;

	// Gets the compilation log.
	const std::string& getLog() const;

	// Compiles the shader.
	virtual bool compile() = 0;
	
	// Is this shader already compiled?
	bool isCompiled() const;

	// Did this shader throw compile errors?
	bool gotCompileErrors() const;

	virtual ResourceGroup::Enum getResourceGroup() const;

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