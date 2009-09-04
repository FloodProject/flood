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
`* Shader file.
 */

class Shader : public Resource
{
public:
	Shader();
	virtual ~Shader();

private:

	//File* shaderFile;
	ShaderType::Enum type;
};

//-----------------------------------//

} } // end namespaces