/************************************************************************
*
* Flood Project � (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Engine/API.h"
#include "Engine/Terrain/Terrain.h"
#include "Engine/Scene/Geometry.h"
#include "Core/Math/Helpers.h"
#include "Core/Utilities.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(Terrain, Group) 
REFLECT_CLASS_END()

//-----------------------------------//

TerrainSettings::TerrainSettings()
	: CellSize(4096)
	, MaxHeight(0)
	, NumberTiles(128)
	, TextureSize(512)
{ }

//-----------------------------------//

Terrain::Terrain()
	: settings( TerrainSettings() )
{
	init();
}

//-----------------------------------//

Terrain::Terrain(const String& name)
	: Group(name)
	, settings( TerrainSettings() )
{
	init();
}

//-----------------------------------//

Terrain::Terrain( const String& name, const TerrainSettings& settings )
	: Group(name)
	, settings( settings )
{ }

//-----------------------------------//

Terrain::~Terrain()
{

}

//-----------------------------------//

void Terrain::init()
{
	MaterialHandle handle = MaterialCreate(AllocatorGetHeap(), "Terrain");
	settings.Material = handle;

	Material* material = handle.Resolve();
	material->setShader("VertexLit");
}

//-----------------------------------//

void Terrain::addCell( int x, int y )
{
	int numTiles = int(settings.NumberTiles + 1.0f);
	int numHeights = numTiles*numTiles;

	Vector<float> heights;
	heights.Resize( numHeights, 0 );

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

	requestsQueue.Push( request );
}

//-----------------------------------//

CellPtr Terrain::getCell( int x, int y )
{
	for( size_t i = 0; i < terrainCells.Size(); i++ )
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

CellPtr Terrain::createCell( int x, int y, Vector<float>& heights )
{
	CellPtr cell = AllocateHeap(Cell, x, y);
	cell->setSettings(settings);
	cell->setHeights(heights);

	terrainCells.Push(cell);

	String name = StringFormat("Cell (%d,%d)", x, y);

	EntityPtr entityCell = EntityCreate(AllocatorGetHeap());
	entityCell->setName(name);
	entityCell->addTransform();
	entityCell->addComponent(cell);

	add( entityCell );

	return cell;
}

//-----------------------------------//

CellPtr Terrain::createCellHeightmap( int x, int y, const ImagePtr& heightmap )
{
	if( !heightmap )
		return nullptr;

	if( !heightmap->isLoaded() )
		return nullptr;

	if( !validateHeightmap(heightmap) )
		return nullptr;

	settings.NumberTiles = heightmap->getWidth() - 1;

	Vector<float> heights;
	convertHeightmap( heightmap, heights );

	return createCell(x, y, heights);
}

//-----------------------------------//

void Terrain::convertHeightmap( const ImagePtr& heightmap, Vector<float>& heights )
{
	#pragma TODO("Can't handle any other pixel format right now...")

	assert( heightmap->getPixelFormat() == PixelFormat::R8G8B8A8 );

	const Vector<byte> data = heightmap->getBuffer();

	for( size_t i = 0; i < data.Size(); i += 4 )
	{
		byte R = data[i];
		byte G = data[i+1];
		byte B = data[i+2];

		float S = 255*3;
		float height = (R/S)+(G/S)+(B/S);
		
		heights.Push( height );
	}
}

//-----------------------------------//

// Range of valid dimensions for heightmaps.
static const uint16 gs_HeightmapDimensions[] =
{
	2,		3,		5,		9,
	17,		33,		65,		129,
	257,	513,	1025,	2049
};

bool Terrain::validateHeightmap( const ImagePtr& heightmap )
{
	if( !heightmap ) return false;
	
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
	for( size_t i = 0; i < FLD_ARRAY_SIZE(gs_HeightmapDimensions); i++ )
	{
		if( width == gs_HeightmapDimensions[i] ) 
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
	Vector<CellRequest>::Iterator it = requestsQueue.begin();

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
			it = requestsQueue.Erase(it);
		}
		else
		{
			it++;
		}
	}

	Group::update( delta );
}

//-----------------------------------//

NAMESPACE_ENGINE_END