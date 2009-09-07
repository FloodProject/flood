/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

#include "vapor/render/Material.h"

namespace vapor {
	namespace render {

//-----------------------------------//

Material::Material( const std::string& name )
	: name( name )
{

}

//-----------------------------------//

Material::~Material()
{

}

//-----------------------------------//

const std::string& Material::getName()
{
	return name;
}

//-----------------------------------//

void Material::addTexture( TexturePtr tex )
{
	textures.push_back( tex );
}

//-----------------------------------//

} } // end namespaces