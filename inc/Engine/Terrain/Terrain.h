/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Engine/Scene/Group.h"
#include "Graphics/Resources/Image.h"
#include "Engine/Terrain/Cell.h"
#include "Engine/Terrain/Brush.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

/**
 * This includes all the terrain settings.
 */

struct API_ENGINE TerrainSettings
{
	TerrainSettings();

	// Material for each cell.
	MaterialHandle Material;

	// Cell size in world units.
	int CellSize;

	// Scales the terrain to this maximum height.
	int MaxHeight;

	// Number of tiles per row/column.
	int NumberTiles;

	// Texture size.
	int TextureSize;
};

//-----------------------------------//

struct API_ENGINE CellRequest
{
	ImagePtr image;
	int x;
	int y;
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

API_ENGINE REFLECT_DECLARE_CLASS(Terrain)

class API_ENGINE Terrain : public Group
{
	REFLECT_DECLARE_OBJECT(Terrain)

public:

	Terrain();
	Terrain( const String& name );
	Terrain( const String& name, const TerrainSettings& settings );

	~Terrain();

	// Initializes the terrain settings.
	void init();

	// Adds a new cell of terrain with default heights.
	void addCell( int x, int y );

	// Adds a new cell of terrain (deferred if the heightmap is not loaded).
	void addCell( int x, int y, const ImagePtr& heightmap );

	// Gets a cell from its terrain coords.
	CellPtr getCell( int x, int y );

	// Gets the cells of the terrain.
	GETTER(Cells, const Array<CellPtr>&, terrainCells)

	// Gets a cell from its world space coords.
	Vector2i getCoords( const Vector3& point );

	// Converts the heightmap to a vector of heights.
	void convertHeightmap( const ImagePtr& heightmap, Array<float>& heights );

	// Valides the heightmap (checks if it has valid dimensions).
	bool validateHeightmap( const ImagePtr& heightmap );

	// Updates the terrain geometry if needed.
	virtual void update( float delta ) OVERRIDE;

protected:

	// Creates a new cell of terrain with the given heights.
	CellPtr createCell(int x, int y, Array<float>& heights);

	// Creates a new cell of terrain with a given heightmap.
	CellPtr createCellHeightmap( int x, int y, const ImagePtr& heightmap );

	// Cells of the terrain.
	Array<CellPtr> terrainCells;

	// Holds temporary cell creation requests.
	Array<CellRequest> requestsQueue;

	// Terrain settings.
	TerrainSettings settings;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Terrain );

//-----------------------------------//

NAMESPACE_ENGINE_END
