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

namespace vapor {
	namespace render {

//-----------------------------------//

using namespace vapor::resources;

//-----------------------------------//

Material::Material( const std::string& name )
	: name( name )
{

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

void Material::addTexture( uint unit, const std::string& tex )
{
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

const std::map< uint, std::string >& Material::getTextures() const
{
	return textures;
}

//-----------------------------------//

void Material::bind()
{
	foreach( const texPair& p, textures )
	{
		ResourceManager* rm = ResourceManager::getInstance();

		std::map<std::string, TexturePtr>::iterator it;

		if( texCache.find( p.second ) == texCache.end() )
		{
			ImagePtr img = rm->loadResource<Image>(p.second);
			TexturePtr tex( new Texture( img ) );
			texCache[p.second] = tex;
		}

		texCache[p.second]->bind(p.first);
	}

	if( !program->isLinked() )
	{
		program->link();

		foreach( const texPair& p, textures )
		{
			program->addUniform( "tex" + 
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
		texCache[p.second]->unbind();
	}
}

//-----------------------------------//

} } // end namespaces