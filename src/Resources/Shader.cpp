/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "Resources/Shader.h"

namespace vapor {

//-----------------------------------//

BEGIN_ENUM(ShaderType)
	ENUM(Vertex)
	ENUM(Fragment)
	ENUM(Geometry)
END_ENUM()

BEGIN_CLASS_PARENT_ABSTRACT(Shader, Resource)
	FIELD_ENUM(ShaderType, shaderType)
END_CLASS()

//-----------------------------------//

Shader::Shader()
	: compiled(false)
	, compileErrors(false)
	, shaderType( ShaderType::Vertex )
{ }

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