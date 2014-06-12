/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Graphics/API.h"
#include "GLSL_Shader.h"
#include "GL.h"

#ifdef ENABLE_RENDERER_OPENGL_GLSL

NAMESPACE_GRAPHICS_BEGIN

//-----------------------------------//

GLSL_Shader::GLSL_Shader() 
	: created(false)
{
}

//-----------------------------------//

GLSL_Shader::~GLSL_Shader()
{
	glDeleteShader(id);
	CheckLastErrorGL("Could not delete shader object");
}

//-----------------------------------//

bool GLSL_Shader::create()
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

bool GLSL_Shader::compile()
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

bool GLSL_Shader::upload()
{
	if( text.Empty() )  return false;

	const char* str = text.CString();
	glShaderSource(id, 1, &str, nullptr);

	if( CheckLastErrorGL("Error uploading shader text to object") )
		return false;

	return true;
}

//-----------------------------------//

void GLSL_Shader::getCompileLog()
{
	// Get compilation log size.
	GLint size;
	glGetShaderiv(id, GL_INFO_LOG_LENGTH, &size);

	GLsizei length;

	// Store the info into the log.
	log.Resize(size);
    glGetShaderInfoLog(id, log.Length(), &length, (GLchar*) log.CString());

	if( log.Empty() )
		log = "Shader source compiled with success";
}

//-----------------------------------//

NAMESPACE_GRAPHICS_END

#endif