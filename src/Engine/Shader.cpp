/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/render/Shader.h"

namespace vapor {

//-----------------------------------//

const std::string ShaderType::getString(Enum e) 
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

//-----------------------------------//

Shader::Shader()
	: compiled( false ), compileErrors( false ),
	type( ShaderType::Vertex )
{

}

//-----------------------------------//

void Shader::forceRecompile()
{
	compiled = false;
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

} // end namespace