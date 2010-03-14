/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/render/Material.h"
#include "vapor/render/ProgramManager.h"
#include "vapor/Utilities.h"

using namespace vapor::resources;

namespace vapor { namespace render {

//-----------------------------------//

static float DEFAULT_LINE_WIDTH = 1.0f;

//-----------------------------------//

Material::Material( const std::string& name, ProgramPtr program )
	: name( name ), program( program ), _isBlendingEnabled( false ),
	src( BlendingOperationSource::One ), dst( BlendingOperationDestination::Zero ),
	lineWidth( DEFAULT_LINE_WIDTH ), lineSmooth( false ), cullBackfaces( true ),
	depthTest( true )
{
}

//-----------------------------------//

Material::Material( const std::string& name, const std::string& program )
	: name( name ), _isBlendingEnabled( false ), 
	program( ProgramManager::getInstance().getProgram( program ) ),
	src( BlendingOperationSource::One ), dst( BlendingOperationDestination::Zero ),
	lineWidth( DEFAULT_LINE_WIDTH ), lineSmooth( false ), cullBackfaces( true ),
	depthTest( true )
{
}

//-----------------------------------//

void Material::setTexture( uint unit, const std::string& name )
{
	TexturePtr tex = TextureManager::getInstance().getTexture( name );
	textures[unit] = tex;
}

//-----------------------------------//

void Material::setTexture( uint unit, TexturePtr tex )
{
	textures[unit] = tex;
}

//-----------------------------------//

bool Material::isBlendingEnabled()
{
	return _isBlendingEnabled;
}

//-----------------------------------//

void Material::setBlending( BlendingOperationSource::Enum src, 
	BlendingOperationDestination::Enum dst )
{
	this->src = src;
	this->dst = dst;
	
	_isBlendingEnabled = true;
}

//-----------------------------------//

void Material::setProgram( const std::string& name )
{
	ProgramPtr p = ProgramManager::getInstance().getProgram( name );
	program = p;
}

//-----------------------------------//

void Material::bind()
{
	foreach( const TextureMapPair& p, textures )
	{
		p.second->bind( p.first );
	}

	if( !program->isLinked() )
		program->link();

	program->bind();

	foreach( const TextureMapPair& p, textures )
	{
		program->setUniform( "tex" + num_to_str(p.first), p.first );
	}

	if( lineSmooth ) 
	{
		glEnable( GL_LINE_SMOOTH );
	}

	if( lineWidth != DEFAULT_LINE_WIDTH ) 
	{
		glLineWidth( lineWidth );
	}

	if( !cullBackfaces )
	{
		glDisable( GL_CULL_FACE );
	}

	if( !depthTest )
	{
		glDisable( GL_DEPTH_TEST );
	}

	if( isBlendingEnabled() ) 
	{
		glEnable( GL_BLEND );
		glBlendFunc( src, dst );
	}
}

//-----------------------------------//

void Material::unbind()
{
	if( isBlendingEnabled() ) 
	{
		glDisable( GL_BLEND );
	}

	if( !cullBackfaces )
	{
		glEnable( GL_CULL_FACE );
	}

	if( !depthTest )
	{
		glEnable( GL_DEPTH_TEST );
	}

	if( lineSmooth )
	{
		glDisable( GL_LINE_SMOOTH );
	}

	if( lineWidth != DEFAULT_LINE_WIDTH ) 
	{
		glLineWidth( DEFAULT_LINE_WIDTH );
	} 

	program->unbind();

	foreach( const TextureMapPair& p, getTextures() )
	{
		p.second->unbind( p.first );
	}
}

//-----------------------------------//

} } // end namespaces