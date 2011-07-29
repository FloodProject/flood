/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Resources/API.h"
#include "Resources/Shader.h"

NAMESPACE_RESOURCES_BEGIN

//-----------------------------------//

REFLECT_ENUM(ShaderType)
	ENUM(Vertex)
	ENUM(Fragment)
	ENUM(Geometry)
REFLECT_ENUM_END()

REFLECT_ABSTRACT_CHILD_CLASS(Shader, Resource)
	FIELD_ENUM(ShaderType, type)
REFLECT_CLASS_END()

//-----------------------------------//

Shader::Shader()
	: compiled(false)
	, compileErrors(false)
	, type( ShaderType::Vertex )
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

NAMESPACE_RESOURCES_END