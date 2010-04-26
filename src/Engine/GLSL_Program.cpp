/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"

#ifdef VAPOR_SHADER_GLSL

#include "vapor/render/GLSL_Program.h"
#include "vapor/render/GL.h"

using namespace vapor::resources;

namespace vapor { namespace render {

//-----------------------------------//

GLSL_Program::GLSL_Program( const GLSL_ShaderPtr& vs, const GLSL_ShaderPtr& ps )
	: Program( vs, ps ), linkError( false )
{
	create();

	//assert( vs && ps );

	if(vs)
	{
		shaders.push_back( vs );
		attached[vs] = false;
	}

	if(ps)
	{
		shaders.push_back( ps );
		attached[ps] = false;
	}
}

//-----------------------------------//

GLSL_Program::~GLSL_Program()
{
	// Detach shaders.
	foreach( const GLSL_ShaderPtr& shader, shaders )
	{
		if( attached[shader] )
			glDetachShader( id, shader->id() );

		if( glHasError("Could not detach shader object") )
			continue;
	}

	glDeleteProgram( id );

	if( glHasError("Could not delete program object") )
		return;
}

//-----------------------------------//

bool GLSL_Program::create()
{
	id = glCreateProgram( );

	if( glHasError( "Could not create a new program object" ) )
		return false;

	return true;
}

//-----------------------------------//

bool GLSL_Program::attachShaders()
{
	// Make sure all shaders are compiled.
	foreach( const GLSL_ShaderPtr& shader, shaders )
	{
		assert( shader->isLoaded() );

		if( !shader->isCompiled() )
		{
			if( !shader->compile() )
			{
				linked = false;
				return false;
			}
		}

		// Shaders need to be attached to the program.
		glAttachShader( id, shader->id() );
		attached[shader] = true;
	}

	return true;
}

//-----------------------------------//

bool GLSL_Program::link()
{
	// If the program is already linked, return.
	if( isLinked() ) return true;

	// If we already tried to link and were not succesful, 
	// don't try to link again until the program is updated.
	if( linkError ) return false;

	// No shaders, don't try to link.
	if( shaders.empty() ) return false;

	// If the shaders aren't loaded, don't try to link.
	foreach( const ShaderPtr& shader, shaders )
	{
		if( !shader->isLoaded() ) 
			return false;
	}

	if( !attachShaders() )
		return false;

	bindDefaultAttributes();

	glLinkProgram( id );

	// Check that the linking was good
	if( glHasError("Could not link program object") )
	{
		linked = false;
		linkError = true;
		return false;
	}

	getLogText();

	int status;
	glGetProgramiv( id, GL_LINK_STATUS, &status );

	if( status != GL_TRUE )
	{
		warn( "glsl", "Could not link program object '%d': %s", id, log.c_str() );
		linked = false;
		linkError = true;
		return false;
	}

	linked = true;
	linkError = false;

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

	if( glHasError("Could not bind program object") )
		return;
}

//-----------------------------------//

void GLSL_Program::unbind()
{
	glUseProgram( 0 );

	if( glHasError("Could not unbind program object") )
		return;
}

//-----------------------------------//

void GLSL_Program::getLogText()
{
	// get linking log size
	GLint size;
	glGetProgramiv( id, GL_INFO_LOG_LENGTH, &size );

	if( size == 0 )
	{
		log = "(no log message)";
		return;
	}

	// TODO: move directly to string...

	GLchar* info = new char[size];
	GLsizei length;

	glGetProgramInfoLog( id, size, &length, info );

	log.assign( info );
	delete info;
}

//-----------------------------------//

void GLSL_Program::bindDefaultAttributes()
{
	// TODO: don't try to bind if user didn't use in shader.

	setAttribute( "vp_Vertex", VertexAttribute::Position );
	setAttribute( "vp_Normal", VertexAttribute::Normal );
	setAttribute( "vp_Color", VertexAttribute::Color );
	setAttribute( "vp_MultiTexCoord0", VertexAttribute::MultiTexCoord0 );
}

//-----------------------------------//

void GLSL_Program::setAttribute( const std::string& name, VertexAttribute::Enum attr )
{
	//if( !isLinked() ) return;

	//bind();

	glBindAttribLocation( id, attr, name.c_str() );

	if( glHasError("Could not bind attribute variable") )
		return;

	//unbind();
}

//-----------------------------------//

void GLSL_Program::setUniform( const std::string& slot, int data )
{
	if( !isLinked() ) return;

	//bind();

	GLint loc = glGetUniformLocation( id, slot.c_str() );

	if( loc == -1 )
	{
		warn( "glsl", "Could not locate uniform location in program object '%d'", id );
		return;
	}

	glUniform1i( loc, data );

	//unbind();
}

//-----------------------------------//

void GLSL_Program::setUniform( const std::string& slot, const std::vector<math::Vector3> vec )
{
	if( !isLinked() ) return;

	assert( sizeof(vec[0]) == 3*sizeof(float) );

	//bind();

	GLint loc = glGetUniformLocation( id, slot.c_str() );

	if( loc == -1 )
	{
		warn( "glsl", "Could not locate uniform location in program object '%d'", id );
		return;
	}

	glUniform3fv( loc, vec.size(), reinterpret_cast<const float*>(&vec[0]) );

	//unbind();
}

//-----------------------------------//

void GLSL_Program::setUniform( const std::string& slot, const std::vector<math::Color> vec )
{
	if( !isLinked() ) return;

	assert( sizeof(vec[0]) == 4*sizeof(float) );

	//bind();

	GLint loc = glGetUniformLocation( id, slot.c_str() );

	if( loc == -1 )
	{
		//warn( "glsl", "Could not locate uniform location in program object '%d'", id );
		return;
	}

	glUniform4fv( loc, vec.size(), reinterpret_cast<const float*>(&vec[0]) );

	//unbind();
}

//-----------------------------------//

void GLSL_Program::setUniform( const std::string& slot, const math::Vector3& vec )
{
	if( !isLinked() ) return;

	//bind();

	GLint loc = glGetUniformLocation( id, slot.c_str() );

	if( loc == -1 )
	{
		//warn( "glsl", "Could not locate uniform location in program object '%d'", id );
		return;
	}

	glUniform3f( loc, vec.x, vec.y, vec.z );

	//unbind();
}

//-----------------------------------//

void GLSL_Program::setUniform( const std::string& slot, const math::Matrix4x3& matrix )
{
	if( !isLinked() ) return;

	//bind();

	GLint loc = glGetUniformLocation( id, slot.c_str() );

	if( loc == -1 )
	{
		//warn( "glsl", "Could not locate uniform location in program object '%d'", id );
		return;
	}

	if(glUniformMatrix4x3fv)
	{
		assert( 0 && "Untested, make sure it works!" );
		glUniformMatrix4x3fv( loc, 1, true, &matrix.m11 );
	}
	else
	{
		math::Matrix4x4 mat( matrix );
		glUniformMatrix4fv( loc, 1, true, &mat.m11 );
	}
}

//-----------------------------------//

void GLSL_Program::setUniform( const std::string& slot, const math::Matrix4x4& matrix )
{
	if( !isLinked() ) return;

	//bind();

	GLint loc = glGetUniformLocation( id, slot.c_str() );

	if( loc == -1 )
	{
		//warn( "glsl", "Could not locate uniform location in program object '%d'", id );
		return;
	}

	// TODO: Is this right? Might be related to the ortographic trouble we had earlier.
	glUniformMatrix4fv( loc, 1, true, &matrix.m11 );
	
	//float test[16];
	//glGetUniformfv( id, loc, test );

	//unbind();
}

//-----------------------------------//

} } // end namespaces

#endif