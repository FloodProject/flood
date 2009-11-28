/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/scene/Terrain.h"

namespace vapor { namespace scene {

using namespace vapor::resources;

//-----------------------------------//

const std::string& Terrain::type = "Terrain";

//-----------------------------------//

Terrain::Terrain( ImagePtr heightmap )
{

}

//-----------------------------------//

Terrain::~Terrain()
{

}

//-----------------------------------//

Page* Terrain::createPage( int x, int y )
{
	return nullptr;
}

//-----------------------------------//

resources::ImagePtr Terrain::getHeightmap() const
{
	return heightmap;
}

//-----------------------------------//

void Terrain::setHeightmap( resources::ImagePtr heightmap )
{
	this->heightmap = heightmap;
}

//-----------------------------------//

void Terrain::update( float delta )
{

}

//-----------------------------------//

const std::string& Terrain::getType() const
{
	return type;
}

//-----------------------------------//

} } // end namespaces