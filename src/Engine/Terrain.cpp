/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/terrain/Terrain.h"
#include "vapor/terrain/Cell.h"
#include "vapor/math/Math.h"

namespace vapor { namespace scene {

using namespace vapor::resources;
using namespace vapor::render;

//-----------------------------------//

// Range of valid dimensions for heightmaps.
static const short validDimensions[] =
{
	2, 3, 5, 9, 17, 
	33, 65, 129, 257, 513, 
	1025, 2049, 4097,
};

//-----------------------------------//

const std::string& Terrain::type = "Terrain";

//-----------------------------------//

#define VAPOR_ARRAY_SIZE(a) (sizeof(a)/sizeof(a[0]))

Terrain::Terrain( const TerrainSettings& settings )
	: settings( settings )
{
	cellMaterial.reset( new Material( "PageMaterial" ) );
}

//-----------------------------------//

Terrain::~Terrain()
{
	//foreach( Cell* cell, terrainCells )
		//delete cell;
}

//-----------------------------------//

Cell* Terrain::createCell( ImagePtr heightmap, int x, int y )
{
	if( !validateHeightmap( heightmap ) )
	{
		return nullptr;
	}

	std::vector<float> heights;
	convertHeightmap( heightmap, heights );

	Cell* page = new Cell( settings, heights, x, y );
	terrainCells.push_back( page );

	page->setMaterial( cellMaterial );
	page->setRenderMode( RenderMode::Wireframe );
	addRenderable( render::RenderablePtr( page ) );
	
	return page;
}

//-----------------------------------//

void Terrain::convertHeightmap( resources::ImagePtr heightmap, std::vector<float>& heights )
{
	// TODO: Can't handle any other pixel format right now...
	assert( heightmap->getPixelFormat() == PixelFormat::R8G8B8A8 );

	const std::vector<byte> data = heightmap->getBuffer();

	for( uint i = 0; i < data.size(); i += 4 )
	{
		byte R = data[i];
		byte G = data[i+1];
		byte B = data[i+2];

		float S = 255*3;
		float height = (R/S)+(G/S)+(B/S);
		
		heights.push_back( height );
	}
}

//-----------------------------------//

bool Terrain::validateHeightmap( ImagePtr heightmap )
{
	const ushort width = heightmap->getWidth();
	const ushort height = heightmap->getHeight();

	// Check the heightmap for the right dimensions.
	// First condition: equal values.
	if( width != height )
	{
		warn( "terrain", "Invalid heightmap (width != height)" );
		return false;
	}

	// That means that they have 2^n+1 dimensions.
	// We built a table previously with the valid dimensions.
	
	bool valid = false;
	for( int i = 0; i < VAPOR_ARRAY_SIZE(validDimensions); i++ )
	{
		if( width == validDimensions[i] ) 
		{
			valid = true;
			break;
		}
	}

	if( !valid )
	{
		warn( "terrain", "Invalid heightmap dimensions (should be 2^n+1)" );
		return false;
	}

	return true;
}

//-----------------------------------//

render::MaterialPtr Terrain::getMaterial() const
{
	return cellMaterial;
}

//-----------------------------------//

void Terrain::setMaterial( render::MaterialPtr material )
{
	this->cellMaterial = material;
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

void Terrain::update( float UNUSED(delta) )
{

}

//-----------------------------------//

const std::string& Terrain::getType() const
{
	return type;
}

//-----------------------------------//

} } // end namespaces