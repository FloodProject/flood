/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

#include "vapor/render/Material.h"

#include <boost/lexical_cast.hpp>

namespace vapor {
	namespace render {

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

void Material::addTexture( uint unit, TexturePtr tex )
{
	textures[unit] = tex;
}

//-----------------------------------//

ProgramPtr Material::getProgram() const
{
	return program;
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
		p.second->unbind();
	}
}

//-----------------------------------//

} } // end namespaces