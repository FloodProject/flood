/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"
#include "vapor/scene/Geometry.h"
#include "vapor/resources/Image.h"

#include "vapor/terrain/Brush.h"

namespace vapor { namespace scene {

	class Cell;

//-----------------------------------//

/**
 * This includes all the terrain settings.
 */

struct TerrainSettings
{
	TerrainSettings()
		:	CellSize( 500 ), TileDimensions( 16 )
	{ }

	// Cell size in world units.
	short CellSize;

	// Number of chunks per row/column.
	short TileDimensions;

	// Scales the terrain to this maximum height.
	short MaxHeight;

	// Material for each cell.
	//render::MaterialPtr CellMaterial;
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
 * for any integer n value that you give.
 */

class VAPOR_API Terrain : public Geometry
{
public:

	Terrain( const TerrainSettings& settings );
	virtual ~Terrain();

	// Creates a new page of terrain.
	Cell* createCell( resources::ImagePtr heightmap, int x, int y );

	// Converts the heightmap to a vector of heights.
	void convertHeightmap( resources::ImagePtr heightmap, std::vector<float>& heights );

	// Valides the heightmap (checks if it has valid dimensions).
	bool validateHeightmap( resources::ImagePtr heightmap );

	// Applies a brush operation in a specified region.
	//void applyBrush( const Brush& brush, const Region& region );

	// Gets the material of the terrain.
	render::MaterialPtr getMaterial() const;

	// Sets the material of the terrain.
	void setMaterial( render::MaterialPtr material );

	// Gets the heightmap of the terrain.
	resources::ImagePtr getHeightmap() const;

	// Sets the heightmap of the terrain.
	void setHeightmap( resources::ImagePtr heightmap );

	// Updates the terrain geometry if needed.
	virtual void update( double delta );

	// Returns the name of this component.
	virtual const std::string& getType() const;

protected:

	// Provides the heights of the terrain.
	resources::ImagePtr heightmap;

	// Pages of the terrain.
	std::vector< Cell* > terrainCells;

	// Material for this terrain.
	render::MaterialPtr cellMaterial;

	// Terrain settings.
	TerrainSettings settings;

	// Component type.
	static const std::string& type;
};

//-----------------------------------//

TYPEDEF_SHARED_POINTER_FROM_CLASS( Terrain );

//-----------------------------------//

} } // end namespaces
