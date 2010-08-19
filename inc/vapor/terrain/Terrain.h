/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/scene/Geometry.h"
#include "vapor/resources/Image.h"
#include "vapor/terrain/Cell.h"
#include "vapor/terrain/Brush.h"

namespace vapor {

//-----------------------------------//

/**
 * This includes all the terrain settings.
 */

struct TerrainSettings
{
	//DECLARE_CLASS_();

	TerrainSettings()
		: CellSize(500)
		, NumberTiles(16)
	{ }

	// Material for each cell.
	MaterialPtr Material;

	// Cell size in world units.
	ushort CellSize;

	// Scales the terrain to this maximum height.
	ushort MaxHeight;

	// Number of tiles per row/column.
	ushort NumberTiles;
};

//-----------------------------------//

struct CellRequest
{
	ImagePtr image;
	short x;
	short y;
};

//-----------------------------------//

/**
 * A terrain is a piece of geometry that is represented by an heightmap,
 * a small bitmap that has the height in each vertice. A terrain is divided
 * into cells, and cells are further sub-divided into tiles. You can change
 * the default size of the terrain cell and tile, so this is pretty much
 * customizable to any type of terrain needs. A terrain can be edited in
 * realtime using different kinds of operations (raise, lower, smooth)
 * and can also be painted with textures, even with different layers and 
 * smooth transitions (using texture splatting). Cells are aligned like
 * in an horizontal grid and are identified by integer coordinates.
 * An heightmap has to have valid dimensions, which are given by (2^n)+1
 * for any integer value n.
 */

class VAPOR_API Terrain : public Geometry
{
	DECLARE_CLASS_()

public:

	Terrain();
	Terrain( const TerrainSettings& settings );

	// Adds a new cell of terrain with default heights.
	void addCell( short x, short y );

	// Adds a new cell of terrain (deferred if the heightmap is not loaded).
	void addCell( short x, short y, const ImagePtr& heightmap );

	// Gets a cell from its terrain coords.
	CellPtr getCell( short x, short y );

	// Gets a cell from its world space coords.
	Vector2i getCoords( const Vector3& pt );

	// Converts the heightmap to a vector of heights.
	void convertHeightmap( const ImagePtr& heightmap, std::vector<float>& heights );

	// Valides the heightmap (checks if it has valid dimensions).
	bool validateHeightmap( const ImagePtr& heightmap );

	// Applies a brush operation in a specified region.
	//void applyBrush( const Brush& brush, const Region& region );

	// Updates the terrain geometry if needed.
	virtual void update( double delta );

protected:

	// Creates a new cell of terrain with the given heights.
	CellPtr Terrain::createCell(short x, short y, std::vector<float>& heights);

	// Creates a new cell of terrain with a given heightmap.
	CellPtr createCellHeightmap( short x, short y, const ImagePtr& heightmap );

	// Cells of the terrain.
	std::vector<CellPtr> terrainCells;

	// Holds temporary cell creation requests.
	std::list<CellRequest> requestsQueue;

	// Terrain settings.
	TerrainSettings settings;
};

TYPEDEF_SHARED_POINTER_FROM_TYPE( Terrain );

//-----------------------------------//

} // end namespace
