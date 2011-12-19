/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Graphics/API.h"
#include "GLSL_ShaderProgram.h"
#include "GL.h"

#ifdef ENABLE_RENDERER_OPENGL_GLSL

NAMESPACE_GRAPHICS_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(GLSL_ShaderProgram, Shader)
REFLECT_CLASS_END()

//-----------------------------------//

GLSL_ShaderProgram::GLSL_ShaderProgram() 
	: created(false)
{
}

//-----------------------------------//

GLSL_ShaderProgram::~GLSL_ShaderProgram()
{
	glDeleteShader(id);
	CheckLastErrorGL("Could not delete shader object");
}

//-----------------------------------//

bool GLSL_ShaderProgram::create()
{
	if( created ) return true;

	id = glCreateShader( ConvertShaderTypeGL(type) );

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
		log = "Shader source compiled with success";
}

//-----------------------------------//

NAMESPACE_GRAPHICS_END

#endif