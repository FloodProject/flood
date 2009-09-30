/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

#include "vapor/render/Material.h"
#include "vapor/resources/ResourceManager.h"

#include <boost/lexical_cast.hpp>

using namespace vapor::resources;

namespace vapor {
	namespace render {

//-----------------------------------//

Material::Material( const std::string& name )
	: name( name )
{
	tm = TextureManager::getInstancePtr();
}

//-----------------------------------//

Material::Material( const std::string& name, ProgramPtr program )
	: name( name ), program( program )
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

ProgramPtr Material::getProgram() const
{
	return program;
}

//-----------------------------------//

void Material::setProgram( ProgramPtr program )
{
	this->program = program;
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
		p.second->bind(p.first);
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

	program->bind();
}

//-----------------------------------//

void Material::unbind()
{
	getProgram()->unbind();

	foreach( const texPair& p, getTextures() )
	{
		p.second->unbind();
	}
}

//-----------------------------------//

} } // end namespaces