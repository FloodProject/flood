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
	TerrainSettings()
		:	CellSize( 500 ), NumberTiles( 16 )
	{ }

	// Material for each cell.
	MaterialPtr Material;

	// Cell size in world units.
	ushort CellSize;

	// Number of tiles per row/column.
	ushort NumberTiles;

	// Scales the terrain to this maximum height.
	ushort MaxHeight;
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
public:

	Terrain( const TerrainSettings& settings );

	// Adds a new cell of terrain (this will be deferred if the heightmap is not loaded).
	void addCell( const ImagePtr& heightmap, ushort x, ushort y );

	// Converts the heightmap to a vector of heights.
	void convertHeightmap( const ImagePtr& heightmap, std::vector<float>& heights );

	// Valides the heightmap (checks if it has valid dimensions).
	bool validateHeightmap( const ImagePtr& heightmap );

	// Applies a brush operation in a specified region.
	//void applyBrush( const Brush& brush, const Region& region );

	// Gets/sets the material of the terrain.
	IMPLEMENT_ACESSOR(Material, MaterialPtr, cellMaterial)

	// Gets/sets the heightmap of the terrain.
	IMPLEMENT_ACESSOR(Heightmap, ImagePtr, heightmap)

	// Updates the terrain geometry if needed.
	virtual void update( double delta );

	// Returns the name of this component.
	IMPLEMENT_GETTER(Type, const std::string&, type)

protected:

	// Creates a new cell of terrain (this will be deferred if the heightmap is not loaded).
	CellPtr createCell( const ImagePtr& heightmap, ushort x, ushort y );

	// Provides the heights of the terrain.
	ImagePtr heightmap;

	// Pages of the terrain.
	std::vector<Cell*> terrainCells;

	// Material for this terrain.
	MaterialPtr cellMaterial;

	typedef std::tuple< ImagePtr, ushort, ushort > CellRequest;
	std::list<CellRequest> requestsQueue;

	// Terrain settings.
	TerrainSettings settings;

	// Component type.
	static const std::string& type;
};

TYPEDEF_SHARED_POINTER_FROM_TYPE( Terrain );

//-----------------------------------//

} // end namespace
