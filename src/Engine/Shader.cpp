/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/render/Shader.h"

namespace vapor { namespace render {

//-----------------------------------//

namespace ShaderType
{
	const std::string getString(Enum e) 
	{
		switch(e) 
		{
		case Vertex:
			return "Vertex";
		case Fragment:
			return "Fragment";
		case Geometry:
			return "Geometry";
		default:
			return "";
		}
	}
}

//-----------------------------------//

Shader::Shader()
	: compiled( false ), compileErrors( false ),
	type( ShaderType::Vertex )
{

}

//-----------------------------------//

bool Shader::isCompiled() const
{
	return compiled;
}

//-----------------------------------//

bool Shader::gotCompileErrors() const
{
	return compileErrors;
}

//-----------------------------------//

} } // end namespaces