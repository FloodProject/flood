/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"

#ifdef VAPOR_SHADER_GLSL

#include "vapor/render/GLSL_Shader.h"
#include "vapor/render/GL.h"

namespace vapor {

//-----------------------------------//

GLSL_Shader::GLSL_Shader() 
	: shaderId(0),
	created(false)
{ }

//-----------------------------------//

GLSL_Shader::~GLSL_Shader()
{
	glDeleteShader( shaderId );

	glHasError("Could not delete shader object");

//#ifdef VAPOR_DEBUG
//	GLenum err = glGetError();
//	if( err != GL_NO_ERROR )
//	{
//		warn( "glsl", "Could not delete shader object '%d': %s", 
//			shaderId, /*glErrorString( err )*/"" );
//	}
//#endif
}

//-----------------------------------//

bool GLSL_Shader::create()
{
	if( created )
		return true;

	shaderId = glCreateShader( getGLShaderType(type) );

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
	if( !create() )
		return false;

	if( !upload() )
	{
		log = "Shader source file is empty";
		return false;
	}

	glCompileShader( shaderId );

	if( glHasError("Error compiling shader object") )
	{
		compiled = false;
		return false;
	}

	getGLSLLog();

	GLint status;
	glGetShaderiv( shaderId, GL_COMPILE_STATUS, &status );

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
	glShaderSource( shaderId, 1, &str, nullptr );

	if( glHasError("Error uploading shader text to object") )
		return false;

	return true;
}

//-----------------------------------//

void GLSL_Shader::getGLSLLog()
{
	// get compilation log size
	GLint size;
	glGetShaderiv( shaderId, GL_INFO_LOG_LENGTH, &size );

	// TODO: move directly to string...

	GLchar* info = new char[size];
	GLsizei length;

	glGetShaderInfoLog( shaderId, size, &length, info );

	log.assign( info );

	delete[] info;

	if( log.empty() )
	{
		log = "Shader source compiled with success";
	}
}

//-----------------------------------//

GLenum GLSL_Shader::getGLShaderType( ShaderType::Enum type )
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
		assert( true ); // Should not be reached.
		return GL_VERTEX_SHADER;
	}	
}

//-----------------------------------//

uint GLSL_Shader::id()
{
	return shaderId;
}

//-----------------------------------//

} // end namespace

#endif