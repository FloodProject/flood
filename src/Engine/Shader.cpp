/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/resources/Shader.h"

namespace vapor { namespace resources {

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

Shader::Shader( ShaderType::Enum e, const std::string& text )
	: type( e ), compiled( false ), text( text ), compileErrors( false )
{

}

//-----------------------------------//

Shader::~Shader()
{

}

//-----------------------------------//

ShaderType::Enum Shader::getType() const
{
	return type;
}

//-----------------------------------//

const std::string& Shader::getText() const
{
	return text;
}

//-----------------------------------//

const std::string& Shader::getLog() const
{
	return log;
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

ResourceGroup::Enum Shader::getResourceGroup() const
{
	return ResourceGroup::Shaders; 
}

//-----------------------------------//

} } // end namespaces