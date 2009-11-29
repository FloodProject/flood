/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/terrain/Terrain.h"
#include "vapor/terrain/Page.h"

namespace vapor { namespace scene {

using namespace vapor::resources;
using namespace vapor::render;

//-----------------------------------//

const std::string& Terrain::type = "Terrain";

//-----------------------------------//

Terrain::Terrain()
{
	pageMaterial.reset( new Material( "PageMaterial" ) );
}

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
	Page* page = new Page( x, y );
	terrainPages.push_back( page );

	page->setRenderMode( RenderMode::Wireframe );
	page->setMaterial( pageMaterial );
	addRenderable( render::RenderablePtr( page ) );
	
	return page;
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