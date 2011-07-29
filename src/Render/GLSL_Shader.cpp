/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"

#ifdef ENABLE_SHADER_GLSL

#include "Render/GLSL_Shader.h"
#include "Render/GL.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(GLSL_Shader, Shader)
REFLECT_CLASS_END()

//-----------------------------------//

GLSL_Shader::GLSL_Shader() 
	: id(0)
	, created(false)
{ }

//-----------------------------------//

GLSL_Shader::~GLSL_Shader()
{
	glDeleteShader(id);

	if( glHasError("Could not delete shader object") )
		return;
}

//-----------------------------------//

bool GLSL_Shader::create()
{
	if( created ) return true;

	id = glCreateShader( getGLShaderType(type) );

	if( glHasError("Could not create a new shader object") )
	{
		created = false;
		return false;
	}

	created = true;
	return true;
}

//-----------------------------------//

bool GLSL_Shader::compile()
{
	if( !create() ) return false;

	if( !upload() )
	{
		log = "Shader source file is empty";
		return false;
	}

	glCompileShader(id);

	if( glHasError("Error compiling shader object") )
	{
		compiled = false;
		return false;
	}

	getCompileLog();

	GLint status;
	glGetShaderiv(id, GL_COMPILE_STATUS, &status);

	if( status != GL_TRUE ) 
	{
		compiled = false;
		return false; 
	}

	compiled = true;
	return true;
}

//-----------------------------------//

bool GLSL_Shader::upload()
{
	if( text.empty() ) 
		return false;

	const char* str = text.c_str();
	glShaderSource(id, 1, &str, nullptr);

	if( glHasError("Error uploading shader text to object") )
		return false;

	return true;
}

//-----------------------------------//

void GLSL_Shader::getCompileLog()
{
	// get compilation log size
	GLint size;
	glGetShaderiv(id, GL_INFO_LOG_LENGTH, &size);

	// TODO: move directly to string...

	GLchar* info = new char[size];
	GLsizei length;

	glGetShaderInfoLog(id, size, &length, info);

	log.assign( info );

	delete[] info;

	if( log.empty() )
	{
		log = "Shader source compiled with success";
	}
}

//-----------------------------------//

uint32 GLSL_Shader::getGLShaderType( ShaderType::Enum type )
{
	switch( type )
	{
	case ShaderType::Vertex:
		return GL_VERTEX_SHADER;
	case ShaderType::Fragment:
		return GL_FRAGMENT_SHADER;
	case ShaderType::Geometry:
		return GL_GEOMETRY_SHADER_EXT;
	default:
		return 0;
	}	
}

//-----------------------------------//

NAMESPACE_ENGINE_END

#endif