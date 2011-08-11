/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"

#ifdef ENABLE_SHADER_GLSL

#include "Render/GLSL_Program.h"
#include "Resources/GLSL_Text.h"
#include "Render/GL.h"
#include "Render/UniformBuffer.h"
#include "Core/Utilities.h"
#include "Core/References.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

GLSL_Program::GLSL_Program( const GLSL_TextPtr& text )
	: text(text)
	, linkError(false)
{
	create();
	createShaders();
	updateShadersText();
}

//-----------------------------------//

GLSL_Program::~GLSL_Program()
{
	// Detach shaders.
	for( size_t i = 0; i < shaders.size(); i++ )
	{
		const GLSL_ShaderPtr& shader = shaders[i];
	
		if( attached[shader] )
			glDetachShader( id, shader->id );

		if( CheckLastErrorGL("Could not detach shader object") )
			continue;
	}

	glDeleteProgram( id );

	if( CheckLastErrorGL("Could not delete program object") )
		return;
}

//-----------------------------------//

bool GLSL_Program::create()
{
	id = glCreateProgram();

	if( CheckLastErrorGL( "Could not create a new program object" ) )
		return false;

	return true;
}

//-----------------------------------//

void GLSL_Program::addShader( const GLSL_ShaderPtr& shader )
{
	shaders.push_back( shader );

	if( !attached[shader] )
	{
		glAttachShader( id, shader->id );

		if( CheckLastErrorGL("Could not attach shader") )
			return;

		attached[shader] = true;
	}
}

//-----------------------------------//

void GLSL_Program::createShaders()
{
	if( !text )
		return;

	vertex = Allocate(GLSL_Shader, AllocatorGetHeap());
	vertex->setShaderType( ShaderType::Vertex );
	vertex->create();
	
	fragment = Allocate(GLSL_Shader, AllocatorGetHeap());
	fragment->setShaderType( ShaderType::Fragment );
	fragment->create();

	addShader( (GLSL_ShaderPtr&) vertex );
	addShader( (GLSL_ShaderPtr&) fragment );
}

//-----------------------------------//

void GLSL_Program::updateShadersText()
{
	assert( text != nullptr );
	
	assert( vertex != nullptr );
	vertex->setText( text->getVertexSource() );
	
	assert( fragment != nullptr );
	fragment->setText( text->getFragmentSource() );

	for( size_t i = 0; i < shaders.size(); i++ )
	{
		const ShaderPtr& shader = shaders[i];
		shader->forceRecompile();
	}

	linkError = false;
	linked = false;
}

//-----------------------------------//

bool GLSL_Program::attachShaders()
{
	// Make sure all shaders are compiled.
	for( size_t i = 0; i < shaders.size(); i++ )
	{
		const GLSL_ShaderPtr& shader = shaders[i];

		if( shader->isCompiled() ) continue;
		
		Path base = PathGetFile(text->getPath());

		if( !shader->compile() )
		{
			LogError( "Error compiling shader '%s': %s",
				base.c_str(), shader->getLog().c_str() );

			linkError = true;
			linked = false;

			return false;
		}
		
		LogInfo( "Compiled shader '%s' with no errors", base.c_str() );
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

	if( !attachShaders() ) return false;

	bindDefaultAttributes();

	glLinkProgram( id );

	// Check that the linking was good
	if( CheckLastErrorGL("Could not link program object") )
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
		LogWarn( "Could not link program object '%d': %s", id, log.c_str() );
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

		LogWarn( "Could not validate program object '%d': %s", id, log.c_str() );
		return false;
	}

	return true;
}

//-----------------------------------//

void GLSL_Program::bind()
{
	glUseProgram( id );

	if( CheckLastErrorGL("Could not bind program object") )
		return;
}

//-----------------------------------//

void GLSL_Program::unbind()
{
	glUseProgram( 0 );

	if( CheckLastErrorGL("Could not unbind program object") )
		return;
}

//-----------------------------------//

void GLSL_Program::getLogText()
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

void GLSL_Program::bindDefaultAttributes()
{
	#pragma TODO("Shader uniforms and attributes should only be bound if needed")

	setAttribute( "vp_Vertex", VertexAttribute::Position );
	setAttribute( "vp_Normal", VertexAttribute::Normal );
	setAttribute( "vp_Color", VertexAttribute::Color );
	setAttribute( "vp_BoneIndex", VertexAttribute::BoneIndex );
	setAttribute( "vp_TexCoord0", VertexAttribute::TexCoord0 );
}

//-----------------------------------//

void GLSL_Program::setAttribute( const String& name, VertexAttribute::Enum attr )
{
	glBindAttribLocation( id, attr, name.c_str() );

	if( CheckLastErrorGL("Could not bind attribute variable") )
		return;
}

//-----------------------------------//

void GLSL_Program::setUniforms( const UniformBufferPtr& ub )
{
	UniformBufferElements::iterator it;
	
	for( it = ub->elements.begin(); it != ub->elements.end(); it++ )
	{
		UniformBufferElement& element = *it->second;

		GLint location = glGetUniformLocation( id, element.name );
		GLint count = element.count;

		if( location == -1 )
			continue;

		switch(element.type)
		{
		case UniformDataType::Scalar_F:
			glUniform1fv(location, count, (GLfloat*) &element.data);
			break;
		case UniformDataType::Scalar_I:
			glUniform1iv(location, count, (GLint*) &element.data);
			break;
		case UniformDataType::Vector2_F:
			glUniform2fv(location, count, (GLfloat*) &element.data);
			break;
		case UniformDataType::Vector3_F:
			glUniform3fv(location, count, (GLfloat*) &element.data);
			break;
		case UniformDataType::Matrix3_F:
			glUniformMatrix3fv(location, count, false, (GLfloat*) &element.data);
			break;
		case UniformDataType::Matrix4_F:
			glUniformMatrix4fv(location, count, false, (GLfloat*) &element.data);
			break;
		case UniformDataType::Matrix4x3_F:
			glUniformMatrix4x3fv(location, count, false, (GLfloat*) &element.data);
			break;
		default:
			LogWarn("Uniform type is not supported");
			continue;
		}
	}
}

//-----------------------------------//

NAMESPACE_ENGINE_END
#endif