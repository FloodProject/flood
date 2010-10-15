/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/terrain/Terrain.h"
#include "vapor/math/Math.h"

namespace vapor {

//-----------------------------------//

BEGIN_CLASS_PARENT(Terrain, Group) 
END_CLASS()

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
	settings.Material = new Material("Terrain");
	settings.Material->setProgram("Tex_Toon");

	ImagePtr image( new Image(1024, 1024, PixelFormat::R8G8B8A8) );
	image->setColor( Color::LightGrey );

	settings.Material->setTexture(0, image);
}

//-----------------------------------//

void Terrain::addCell( short x, short y )
{
	int numHeights = pow(settings.NumberTiles + 1.0f, 2);

	std::vector<float> heights;
	heights.resize( numHeights, 0 );

	createCell(x, y, heights);
}

//-----------------------------------//

void Terrain::addCell( short x, short y, const ImagePtr& heightmap )
{
	if( !heightmap )
	{
		Log::warn("Cannot create terrain cell: Heightmap is invalid");
		return;
	}

	CellRequest request;
	request.image = heightmap;
	request.x = x;
	request.y = y;

	requestsQueue.push_back( request );
}

//-----------------------------------//

CellPtr Terrain::getCell( short x, short y )
{
	foreach( const CellPtr& cell, terrainCells )
	{
		if( (cell->getX() == x) && (cell->getY() == y) )
			return cell;
	}

	return nullptr;
}

//-----------------------------------//

Vector2i Terrain::getCoords( const Vector3& pos )
{
	int x = floor(pos.x / settings.CellSize); 
	int y = floor(pos.z / settings.CellSize);

	return Vector2i(x, y);
}

//-----------------------------------//

CellPtr Terrain::createCell( short x, short y, std::vector<float>& heights )
{
	CellPtr cell( new Cell(settings, heights, x, y) );
	terrainCells.push_back(cell);

	std::string name = String::format("Cell (%hd,%hd)", x, y);
	NodePtr nodeCell( new Node(name) );
	nodeCell->addTransform();

	GeometryPtr geometry( new Geometry(cell) );
	nodeCell->addComponent(geometry);

	add( nodeCell );

	return cell;
}

//-----------------------------------//

CellPtr Terrain::createCellHeightmap( short x, short y, const ImagePtr& heightmap )
{
	if( !heightmap )
		return nullptr;

	if( !heightmap->isLoaded() )
		return nullptr;

	if( !validateHeightmap(heightmap) )
		return nullptr;

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

bool Terrain::validateHeightmap( const ImagePtr& heightmap )
{
	if( !heightmap )
		return false;
	
	assert( heightmap->isLoaded() );

	const ushort width = heightmap->getWidth();
	const ushort height = heightmap->getHeight();

	// Check the heightmap for the right dimensions.
	// First condition: width and height should be the same.
	if( width != height )
	{
		Log::warn( "Invalid heightmap (width != height)" );
		return false;
	}

	// The dimension should obey the formula 2^n+1.
	// Check an embedded LUT with the valid dimensions.
	
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
		Log::warn( "Invalid heightmap dimensions (should be 2^n+1)" );
		return false;
	}

	return true;
}

//-----------------------------------//

void Terrain::update( double delta )
{
	std::list<CellRequest>::iterator it;
	it = requestsQueue.begin();

	while( it != requestsQueue.end() )
	{
		const CellRequest& request = (*it);
		
		const ImagePtr& heightmap = request.image;
		assert( heightmap != nullptr );

		short x = request.x;
		short y = request.y;
		
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