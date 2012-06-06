/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Graphics/API.h"

#ifdef ENABLE_RENDERER_OPENGL_GLSL

#include "GLSL_Shader.h"
#include "GLSL_ShaderProgram.h"
#include "GL.h"
#include "Graphics/UniformBuffer.h"
#include "Core/Utilities.h"
#include "Core/References.h"

NAMESPACE_GRAPHICS_BEGIN

//-----------------------------------//

GLSL_ShaderProgram::GLSL_ShaderProgram()
	: hadLinkError(false)
{
	create();
	createShaders();
}

//-----------------------------------//

GLSL_ShaderProgram::~GLSL_ShaderProgram()
{
	detachShaders();

	//Deallocate(vertex);
	//Deallocate(fragment);

	shaders.clear();

	glDeleteProgram( id );

	if( CheckLastErrorGL("Could not delete program object") )
		return;
}

//-----------------------------------//

bool GLSL_ShaderProgram::create()
{
	id = glCreateProgram();

	if( CheckLastErrorGL( "Could not create a new program object" ) )
		return false;

	return true;
}

//-----------------------------------//

void GLSL_ShaderProgram::addShader( GLSL_Shader* shader)
{
	shaders.push_back( shader );
	
	bool isAttached = attached[shader];
	
	if( !isAttached )
	{
		glAttachShader( id, shader->id );

		if( CheckLastErrorGL("Could not attach shader object") )
			return;

		attached[shader] = true;
	}
}

//-----------------------------------//

void GLSL_ShaderProgram::detachShaders()
{
	for( size_t i = 0; i < shaders.size(); i++ )
	{
		GLSL_Shader* shader = shaders[i].get();
		bool isAttached = attached[shader];
		
		if( !isAttached ) continue;

		glDetachShader( id, shader->id );
		CheckLastErrorGL("Could not detach shader object");
	}

	attached.clear();
}

//-----------------------------------//

void GLSL_ShaderProgram::createShaders()
{
	vertex = AllocateThis(GLSL_Shader);
	vertex->setShaderType( ShaderType::Vertex );
	vertex->create();
	
	fragment = AllocateThis(GLSL_Shader);
	fragment->setShaderType( ShaderType::Fragment );
	fragment->create();

	addShader( (GLSL_Shader*) vertex );
	addShader( (GLSL_Shader*) fragment );
}

//-----------------------------------//

bool GLSL_ShaderProgram::compileShaders()
{
	for( size_t i = 0; i < shaders.size(); i++ )
	{
		Shader* shader = shaders[i].get();
		if( shader->isCompiled() ) continue;
		
		if( !shader->compile() )
		{
			LogError( "Error compiling shader program: %s", shader->getLog().c_str() );

			hadLinkError = true;
			linked = false;

			return false;
		}
		
		LogInfo( "Compiled shader program with no errors");
	}

	return true;
}

//-----------------------------------//

void GLSL_ShaderProgram::forceRecompile()
{
	for( size_t i = 0; i < shaders.size(); i++ )
	{
		Shader* shader = shaders[i].get();
		shader->forceRecompile();
	}

	linked = false;
	hadLinkError = false;
}

//-----------------------------------//

bool GLSL_ShaderProgram::link()
{
	if( isLinked() ) return true;

	// If we already tried to link and were not succesful return.
	if( hadLinkError ) return false;

	// If there are no shader programs, no point in trying to link.
	if( shaders.empty() ) return false;

	// If we could not compile the shaders, no point in trying to link.
	if( !compileShaders() ) return false;

	bindDefaultAttributes();

	glLinkProgram( id );

	// Check that the linking was good
	if( CheckLastErrorGL("Could not link program object") )
	{
		linked = false;
		hadLinkError = true;
		return false;
	}

	getLogText();

	GLint status;
	glGetProgramiv( id, GL_LINK_STATUS, &status );

	if( status != GL_TRUE )
	{
		LogWarn( "Could not link program object '%d': %s", id, log.c_str() );
		linked = false;
		hadLinkError = true;
		return false;
	}

	linked = true;
	hadLinkError = false;

	return true;
}

//-----------------------------------//

bool GLSL_ShaderProgram::validate()
{
	if( !isLinked() ) return false;
	
	glValidateProgram( id );
	
	int status;
	glGetProgramiv( id, GL_VALIDATE_STATUS, &status );

	if( status != GL_TRUE )
	{
		getLogText();

		LogWarn( "Could not validate program object '%d': %s", id, log.c_str() );
		return false;
	}

	return true;
}

//-----------------------------------//

void GLSL_ShaderProgram::bind()
{
	glUseProgram( id );

	if( CheckLastErrorGL("Could not bind program object") )
		return;
}

//-----------------------------------//

void GLSL_ShaderProgram::unbind()
{
	glUseProgram( 0 );

	if( CheckLastErrorGL("Could not unbind program object") )
		return;
}

//-----------------------------------//

void GLSL_ShaderProgram::getLogText()
{
	GLint length;
	glGetProgramiv( id, GL_INFO_LOG_LENGTH, &length );

	if( length == 0 )
	{
		log.clear();
		return;
	}

	log.resize(length);

	GLsizei temp;
	glGetProgramInfoLog( id, log.size(), &temp, &log[0] );
}

//-----------------------------------//

void GLSL_ShaderProgram::bindDefaultAttributes()
{
	#pragma TODO("Shader uniforms and attributes should only be bound if needed")

	setAttribute( "vp_Vertex", VertexAttribute::Position );
	setAttribute( "vp_Normal", VertexAttribute::Normal );
	setAttribute( "vp_Color", VertexAttribute::Color );
	setAttribute( "vp_BoneIndex", VertexAttribute::BoneIndex );
	setAttribute( "vp_TexCoord0", VertexAttribute::TexCoord0 );
}

//-----------------------------------//

void GLSL_ShaderProgram::setAttribute( const String& name, VertexAttribute::Enum attr )
{
	glBindAttribLocation( id, attr, name.c_str() );

	if( CheckLastErrorGL("Could not bind attribute variable") )
		return;
}

//-----------------------------------//

void GLSL_ShaderProgram::setUniforms( UniformBuffer* ub )
{
	UniformBufferElements::iterator it;
	
	for( it = ub->elements.begin(); it != ub->elements.end(); it++ )
	{
		UniformBufferElement* element = it->second;
		if( !element || !element->name ) continue;

		GLint location = glGetUniformLocation( id, /*element->name*/it->first.c_str() );
		if( location == -1 ) continue;

		GLint count = element->count;

		switch(element->type)
		{
		case UniformDataType::Scalar_F:
			glUniform1fv(location, count, (GLfloat*) &element->data);
			break;
		case UniformDataType::Scalar_I:
			glUniform1iv(location, count, (GLint*) &element->data);
			break;
		case UniformDataType::Vector2_F:
			glUniform2fv(location, count, (GLfloat*) &element->data);
			break;
		case UniformDataType::Vector3_F:
			glUniform3fv(location, count, (GLfloat*) &element->data);
			break;
		case UniformDataType::Matrix3_F:
			glUniformMatrix3fv(location, count, false, (GLfloat*) &element->data);
			break;
		case UniformDataType::Matrix4_F:
			glUniformMatrix4fv(location, count, false, (GLfloat*) &element->data);
			break;
		case UniformDataType::Matrix4x3_F:
			glUniformMatrix4x3fv(location, count, false, (GLfloat*) &element->data);
			break;
		default:
			LogWarn("Uniform type is not supported");
			continue;
		}
	}
}

//-----------------------------------//

NAMESPACE_GRAPHICS_END
#endif