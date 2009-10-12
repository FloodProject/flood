/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

#ifdef VAPOR_SHADER_GLSL

#include "vapor/render/GLSL_Program.h"

#include "vapor/render/GL.h"

using namespace vapor::resources;

namespace vapor {
	namespace render {

//-----------------------------------//

GLSL_Program::GLSL_Program( GLSL_ShaderPtr vs, GLSL_ShaderPtr ps )
	: Program( vs, ps )
{
	id = glCreateProgram( );

#ifdef VAPOR_DEBUG
	if( glGetError() != GL_NO_ERROR )
	{
		warn( "glsl", "Could not create a new program object" );
		return;
	}
#endif

	shaders.push_back( vs );
	shaders.push_back( ps );
}

//-----------------------------------//

GLSL_Program::~GLSL_Program()
{
	// Detach shaders
	foreach( GLSL_ShaderPtr shader, shaders )
	{
		glDetachShader( id, shader->id() );

#ifdef VAPOR_DEBUG
		if( glGetError() != GL_NO_ERROR )
		{
			warn( "glsl", "Could not detach shader object '%d'", shader->id() );
			return;
		}
#endif
	}

	glDeleteProgram( id );

#ifdef VAPOR_DEBUG
	if( glGetError() != GL_NO_ERROR )
	{
		warn( "glsl", "Could not delete program object '%d'", id );
		return;
	}
#endif
}

//-----------------------------------//

void GLSL_Program::setAttribute( const std::string& name, VertexAttribute::Enum attr )
{
	//if( !isLinked() ) return;

	bind();

	glBindAttribLocation( id, attr, name.c_str() );

#ifdef VAPOR_DEBUG
	if( glGetError() != GL_NO_ERROR )
	{
		warn( "glsl", "Could not bind attribute variable in program object '%d'", id );
	}
#endif

	unbind();
}

//-----------------------------------//

void GLSL_Program::setUniform( const std::string& slot, int data )
{
	if( !isLinked() ) return;

	bind();

	int loc = glGetUniformLocation( id, slot.c_str() );

	if( loc == -1)
	{
		warn( "glsl", "Could not locate uniform location in program object '%d'", id );
		return;
	}

	glUniform1i( loc, data );

	unbind();
}

//-----------------------------------//

void GLSL_Program::setUniform( const std::string& slot, const math::Matrix4& matrix )
{
	if( !isLinked() ) return;

	bind();

	int loc = glGetUniformLocation( id, slot.c_str() );

	if( loc == -1)
	{
		//warn( "glsl", "Could not locate uniform location in program object '%d'", id );
		return;
	}

	float tmp[16];
	tmp[0] = matrix.m11; tmp[1] = matrix.m12; tmp[2] = matrix.m13; tmp[3] = 0;
	tmp[4] = matrix.m21; tmp[5] = matrix.m22; tmp[6] = matrix.m23; tmp[7] = 0;
	tmp[8] = matrix.m31; tmp[9] = matrix.m32; tmp[10] = matrix.m33; tmp[11] = 0;
	tmp[12] = matrix.tx; tmp[13] = matrix.ty; tmp[14] = matrix.tz; tmp[15] = 1;

	// TODO: This crashes on my Intel driver. Is it a driver bug?
	//glUniformMatrix4x3fv( loc, 1, true, &matrix.m11 );

	glUniformMatrix4fv( loc, 1, false, tmp );
	
	//float test[16];
	//glGetUniformfv( id, loc, test );

	unbind();
}

//-----------------------------------//

void GLSL_Program::setUniform( const std::string& slot, const math::Matrix4x4& matrix )
{
	if( !isLinked() ) return;

	bind();

	int loc = glGetUniformLocation( id, slot.c_str() );

	if( loc == -1)
	{
		//warn( "glsl", "Could not locate uniform location in program object '%d'", id );
		return;
	}

	float tmp[16];
	tmp[0] = matrix.m11; tmp[1] = matrix.m12; tmp[2] = matrix.m13; tmp[3] = matrix.m14;
	tmp[4] = matrix.m21; tmp[5] = matrix.m22; tmp[6] = matrix.m23; tmp[7] = matrix.m24;
	tmp[8] = matrix.m31; tmp[9] = matrix.m32; tmp[10] = matrix.m33; tmp[11] = matrix.m34;
	tmp[12] = matrix.tx; tmp[13] = matrix.ty; tmp[14] = matrix.tz; tmp[15] = matrix.tw;

	// the true tells opengl our matrices are in row-major order
	glUniformMatrix4fv( loc, 1, true, tmp );
	
	//float test[16];
	//glGetUniformfv( id, loc, test );

	unbind();
}

//-----------------------------------//

void GLSL_Program::attachShaders()
{
	// Make sure all shaders are compiled
	foreach( GLSL_ShaderPtr shader, shaders )
	{
		if( !shader->isCompiled() )
		{
			if( !shader->compile() )
			{
				linked = false;
			}
		}

		// Shaders need to be attached to the program
		glAttachShader( id, shader->id() );
	}
}

//-----------------------------------//

bool GLSL_Program::link()
{
	// If the program is already linked, return.
	if( isLinked() ) return true;

	attachShaders();

	bindDefaultAttributes();

	glLinkProgram( id );

	// Check that the linking was good
#ifdef VAPOR_DEBUG

	GLenum err;
	if( ( err = glGetError() ) != GL_NO_ERROR )
	{
		warn( "glsl", 
			"Could not link program object '%d': %s", id, gluErrorString( err ) );
		linked = false;
		return false;
	}
#endif

	getLogText();

	int status;
	glGetProgramiv( id, GL_LINK_STATUS, &status );

	if( status != GL_TRUE )
	{
		warn( "glsl", "Could not link program object '%d': %s", id, log.c_str() );
		linked = false;
		return false;
	}

	linked = true;
	return true;
}

//-----------------------------------//

bool GLSL_Program::validate()
{
	if( !isLinked() ) return false;
	
	glValidateProgram( id );
	
	int status;
	glGetProgramiv( id, GL_VALIDATE_STATUS, &status );

	if( status != GL_TRUE )
	{
		getLogText();

		warn( "glsl", "Could not validate program object '%d': %s", id, log.c_str() );
		return false;
	}

	return true;
}


//-----------------------------------//

void GLSL_Program::bind()
{
	if( !isLinked() ) return;

	glUseProgram( id );

#ifdef VAPOR_DEBUG
	if( glGetError() != GL_NO_ERROR )
	{
		warn( "glsl", "Could not bind program object '%d'", id );
		return;
	}
#endif	
}

//-----------------------------------//

void GLSL_Program::unbind()
{
	glUseProgram( 0 );

#ifdef VAPOR_DEBUG
	if( glGetError() != GL_NO_ERROR )
	{
		warn( "glsl", "Could not bind program object '%d'", id );
		return;
	}
#endif	
}

//-----------------------------------//

void GLSL_Program::getLogText()
{
	// get linking log size
	GLint size;
	glGetProgramiv( id, GL_INFO_LOG_LENGTH, &size );

	if( size == 0 )
	{
		log = "Program linked with success";
		return;
	}

	// TODO: move directly to string...

	GLchar* info = new char[size];
	GLsizei length;

	glGetProgramInfoLog( id, size, &length, info );

	log.assign( info );

	debug( log.c_str() );

	delete info;
}

//-----------------------------------//

void GLSL_Program::bindDefaultAttributes()
{
	setAttribute( "vp_Vertex", VertexAttribute::Vertex );
	setAttribute( "vp_Color", VertexAttribute::Color );
	setAttribute( "vp_MultiTexCoord0", VertexAttribute::MultiTexCoord0 );
}

//-----------------------------------//

//uint GLSL_Program::id()
//{
//	return id;
//}

//-----------------------------------//

} } // end namespaces

#endif