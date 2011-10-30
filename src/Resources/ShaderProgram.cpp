/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Resources/API.h"
#include "Resources/ShaderProgram.h"

NAMESPACE_RESOURCES_BEGIN

//-----------------------------------//

REFLECT_ENUM(ShaderType)
	ENUM(Vertex)
	ENUM(Fragment)
	ENUM(Geometry)
REFLECT_ENUM_END()

REFLECT_ABSTRACT_CHILD_CLASS(ShaderProgram, Resource)
	FIELD_ENUM(4, ShaderType, type)
REFLECT_CLASS_END()

//-----------------------------------//

ShaderProgram::ShaderProgram()
	: compiled(false)
	, compileErrors(false)
	, type( ShaderType::Vertex )
{ }

//-----------------------------------//

void ShaderProgram::forceRecompile()
{
	compiled = false;
}

//-----------------------------------//

bool ShaderProgram::isCompiled() const
{
	return compiled;
}

//-----------------------------------//

bool ShaderProgram::gotCompileErrors() const
{
	return compileErrors;
}

//-----------------------------------//

NAMESPACE_RESOURCES_END