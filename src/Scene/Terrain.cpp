/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"
#include "Terrain/Terrain.h"
#include "Scene/Geometry.h"
#include "Math/Helpers.h"
#include "Core/Utilities.h"

namespace vapor {

//-----------------------------------//

REFLECT_CHILD_CLASS(Terrain, Group) 
REFLECT_CLASS_END()

//-----------------------------------//

// Range of valid dimensions for heightmaps.
static const short validDimensions[] =
{
	2,		3,		5,		9,
	17,		33,		65,		129,
	257,	513,	1025,	2049
};

//-----------------------------------//

Terrain::Terrain()
	: settings( TerrainSettings() )
{
	init();
}

//-----------------------------------//

Terrain::Terrain(const std::string& name)
	: Group(name)
	, settings( TerrainSettings() )
{
	init();
}

//-----------------------------------//

Terrain::Terrain( const std::string& name, const TerrainSettings& settings )
	: Group(name)
	, settings( settings )
{ }

//-----------------------------------//

void Terrain::init()
{
	MaterialHandle handle = MaterialCreate(AllocatorGetHeap(), "Terrain");

	Material* material = handle.Resolve();
	material->setProgram("VertexLit");

	settings.Material = material;
}

//-----------------------------------//

void Terrain::addCell( int x, int y )
{
	int numTiles = int(settings.NumberTiles + 1.0f);
	int numHeights = numTiles*numTiles;

	std::vector<float> heights;
	heights.resize( numHeights, 0 );

	createCell(x, y, heights);
}

//-----------------------------------//

void Terrain::addCell( int x, int y, const ImagePtr& heightmap )
{
	if( !heightmap )
	{
		LogWarn("Cannot create terrain cell: Heightmap is invalid");
		return;
	}

	CellRequest request;
	request.image = heightmap;
	request.x = x;
	request.y = y;

	requestsQueue.push_back( request );
}

//-----------------------------------//

CellPtr Terrain::getCell( int x, int y )
{
	for( size_t i = 0; i < terrainCells.size(); i++ )
	{
		const CellPtr& cell = terrainCells[i];

		if( (cell->getX() == x) && (cell->getY() == y) )
			return cell;
	}

	return CellPtr();
}

//-----------------------------------//

Vector2i Terrain::getCoords( const Vector3& pos )
{
	float x = floor(pos.x / settings.CellSize); 
	float y = floor(pos.z / settings.CellSize);

	return Vector2i( int(x), int(y) );
}

//-----------------------------------//

CellPtr Terrain::createCell( int x, int y, std::vector<float>& heights )
{
	CellPtr cell( new Cell(x, y) );
	cell->setSettings(settings);
	cell->setHeights(heights);

	terrainCells.push_back(cell);

	String name = StringFormat("Cell (%d,%d)", x, y);

	Entity* entity = EntityCreate(AllocatorGetHeap());
	entity->setName(name);

	EntityPtr entityCell(entity);
	entityCell->addTransform();
	entityCell->addComponent(cell);

	add( entityCell );

	return cell;
}

//-----------------------------------//

CellPtr Terrain::createCellHeightmap( int x, int y, const ImagePtr& heightmap )
{
	if( !heightmap )
		return CellPtr();

	if( !heightmap->isLoaded() )
		return CellPtr();

	if( !validateHeightmap(heightmap) )
		return CellPtr();

	settings.NumberTiles = heightmap->getWidth() - 1;

	std::vector<float> heights;
	convertHeightmap( heightmap, heights );

	return createCell(x, y, heights);
}

//-----------------------------------//

void Terrain::convertHeightmap( const ImagePtr& heightmap, std::vector<float>& heights )
{
	#pragma TODO("Can't handle any other pixel format right now...")

	assert( heightmap->getPixelFormat() == PixelFormat::R8G8B8A8 );

	const std::vector<byte> data = heightmap->getBuffer();

	for( size_t i = 0; i < data.size(); i += 4 )
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

bool Terrain::validateHeightmap( const ImagePtr& heightmap )
{
	if( !heightmap )
		return false;
	
	assert( heightmap->isLoaded() );

	int width = heightmap->getWidth();
	int height = heightmap->getHeight();

	// Check the heightmap for the right dimensions.
	// First condition: width and height should be the same.
	if( width != height )
	{
		LogWarn( "Invalid heightmap (width != height)" );
		return false;
	}

	// The dimension should obey the formula 2^n+1.
	// Check an embedded LUT with the valid dimensions.
	
	bool valid = false;
	for( size_t i = 0; i < ARRAY_SIZE(validDimensions); i++ )
	{
		if( width == validDimensions[i] ) 
		{
			valid = true;
			break;
		}
	}

	if( !valid )
	{
		LogWarn( "Invalid heightmap dimensions (should be 2^n+1)" );
		return false;
	}

	return true;
}

//-----------------------------------//

void Terrain::update( float delta )
{
	std::vector<CellRequest>::iterator it = requestsQueue.begin();

	while( it != requestsQueue.end() )
	{
		const CellRequest& request = (*it);
		
		const ImagePtr& heightmap = request.image;
		assert( heightmap != nullptr );

		int x = request.x;
		int y = request.y;
		
		if( heightmap->isLoaded() )
		{
			createCellHeightmap( x, y, heightmap );
			it = requestsQueue.erase(it);
		}
		else
		{
			it++;
		}
	}

	Group::update( delta );
}

//-----------------------------------//

} // end namespace