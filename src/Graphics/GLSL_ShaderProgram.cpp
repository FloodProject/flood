/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"

#ifdef ENABLE_RENDERER_OPENGL_GLSL

#include "Graphics/GLSL_ShaderProgram.h"
#include "Graphics/GL.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(GLSL_ShaderProgram, Shader)
REFLECT_CLASS_END()

//-----------------------------------//

GLSL_ShaderProgram::GLSL_ShaderProgram() 
	: id(0)
	, created(false)
{ }

//-----------------------------------//

GLSL_ShaderProgram::~GLSL_ShaderProgram()
{
	glDeleteShader(id);

	if( CheckLastErrorGL("Could not delete shader object") )
		return;
}

//-----------------------------------//

bool GLSL_ShaderProgram::create()
{
	if( created ) return true;

	id = glCreateShader( getGLShaderType(type) );

	if( CheckLastErrorGL("Could not create a new shader object") )
	{
		created = false;
		return false;
	}

	created = true;
	return true;
}

//-----------------------------------//

bool GLSL_ShaderProgram::compile()
{
	if( !create() ) return false;

	if( !upload() )
	{
		log = "Shader source file is empty";
		return false;
	}

	glCompileShader(id);

	if( CheckLastErrorGL("Error compiling shader object") )
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

bool GLSL_ShaderProgram::upload()
{
	if( text.empty() )  return false;

	const char* str = text.c_str();
	glShaderSource(id, 1, &str, nullptr);

	if( CheckLastErrorGL("Error uploading shader text to object") )
		return false;

	return true;
}

//-----------------------------------//

void GLSL_ShaderProgram::getCompileLog()
{
	// Get compilation log size.

	GLint size;
	glGetShaderiv(id, GL_INFO_LOG_LENGTH, &size);

	GLsizei length;

	// Store the info into the log.

	log.resize(size);
	glGetShaderInfoLog(id, log.size(), &length, (GLchar*) log.data());

	if( log.empty() )
	{
		log = "Shader source compiled with success";
	}
}

//-----------------------------------//

uint32 GLSL_ShaderProgram::getGLShaderType( ShaderType::Enum type )
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