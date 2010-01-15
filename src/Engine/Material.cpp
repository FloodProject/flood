/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

#include "vapor/render/Material.h"
#include "vapor/resources/ResourceManager.h"
#include "vapor/render/ProgramManager.h"

#include <boost/lexical_cast.hpp>

using namespace vapor::resources;

namespace vapor { namespace render {

static float DEFAULT_LINE_WIDTH = 1.0f;

//-----------------------------------//

//Material::Material( const std::string& name )
//	: name( name ), _isBlendingEnabled( false ),
//	src( BlendingOperationSource::One ), dst( BlendingOperationDestination::Zero )
//{
//
//}

//-----------------------------------//

Material::Material( const std::string& name, ProgramPtr program )
	: name( name ), program( program ), _isBlendingEnabled( false ),
	src( BlendingOperationSource::One ), dst( BlendingOperationDestination::Zero ),
	lineWidth( DEFAULT_LINE_WIDTH ), lineSmooth( false )
{

}

//-----------------------------------//

Material::Material( const std::string& name, const std::string& program )
	: name( name ), _isBlendingEnabled( false ), 
	program( ProgramManager::getInstance().getProgram( program ) ),
	src( BlendingOperationSource::One ), dst( BlendingOperationDestination::Zero ),
	lineWidth( DEFAULT_LINE_WIDTH ), lineSmooth( false )
{
}

//-----------------------------------//

Material::~Material()
{

}

//-----------------------------------//

const std::string& Material::getName() const
{
	return name;
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
	//return (src != BlendingOperationSource::Zero)
		//|| (dst != BlendingOperationDestination::SourceColor);

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

BlendingOperationSource::Enum Material::getSourceBlendingOperation()
{
	return src;
}

//-----------------------------------//

BlendingOperationDestination::Enum Material::getDestinationBlendingOperation()
{
	return dst;
}

//-----------------------------------//

ProgramPtr Material::getProgram() const
{
	if( !program )
		warn( "Invalid program in material '%s'", getName().c_str() );
	
	return program;
}

//-----------------------------------//

void Material::setProgram( ProgramPtr program )
{
	this->program = program;
}

//-----------------------------------//

float Material::getLineWidth() const
{
	return lineWidth;
}

//-----------------------------------//

void Material::setLineWidth( float width )
{
	this->lineWidth = width;
}

//-----------------------------------//

bool Material::getLineSmoothing() const
{
	return lineSmooth;
}

//-----------------------------------//

void Material::setLineSmoothing( bool smooth )
{
	this->lineSmooth = smooth;
}

//-----------------------------------//

void Material::setProgram( const std::string& name )
{
	ProgramPtr p = ProgramManager::getInstance().getProgram( name );
	program = p;
}

//-----------------------------------//

const std::map< uint, TexturePtr >& Material::getTextures() const
{
	return textures;
}

//-----------------------------------//

void Material::bind()
{
	foreach( const texPair& p, textures )
	{
		p.second->bind( p.first );
	}

	if( !program->isLinked() )
	{
		program->link();

		foreach( const texPair& p, textures )
		{
			program->setUniform( "tex" + 
				boost::lexical_cast< std::string >( p.first ), p.first );
		}
	}

	if( lineSmooth ) 
	{
		glEnable( GL_LINE_SMOOTH );
	}

	if( lineWidth != DEFAULT_LINE_WIDTH ) 
	{
		glLineWidth( lineWidth );
	}

	program->bind();

	if( isBlendingEnabled() ) 
	{
		glEnable( GL_BLEND );
		glBlendFunc( src, dst );
	}
}

//-----------------------------------//

void Material::unbind()
{
	getProgram()->unbind();

	foreach( const texPair& p, getTextures() )
	{
		p.second->unbind( p.first );
	}
	
	if( isBlendingEnabled() ) 
	{
		glDisable( GL_BLEND );
	}

	glDisable( GL_LINE_SMOOTH );
	glLineWidth( DEFAULT_LINE_WIDTH );
}

//-----------------------------------//

} } // end namespaces