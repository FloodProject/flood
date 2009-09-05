/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"
#include "vapor/resources/Resource.h"

namespace vapor {
	namespace resources {

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

class Shader : public Resource
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

	virtual ResourceGroup::Enum getResourceGroup() const;

	bool isCompiled() const;

protected:

	// Shader type.
	ShaderType::Enum type;

	// Holds the shader text.
	std::string text;

	// Holds the compilation log.
	std::string log;
	
	// Is this shader already compiled?	
	bool compiled;
};

//-----------------------------------//

typedef tr1::shared_ptr< Shader > ShaderPtr;

//-----------------------------------//

} } // end namespaces