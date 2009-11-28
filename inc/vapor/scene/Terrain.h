/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"
#include "vapor/math/Vector3.h"
#include "vapor/scene/Geometry.h"
#include "vapor/resources/Image.h"

#include "vapor/terrain/Brush.h"

namespace vapor { namespace scene {

	class Page;

//-----------------------------------//

/**
 * A terrain is a piece of geometry that is represented by an heightmap,
 * a small bitmap that has the height in each point. A terrain is divided
 * into pages, and pages are further sub-divided into cells. You can change
 * the default size of the terrain, page and cell, so this is pretty much
 * customizable to any type of terrain needs. A terrain can be edited in
 * realtime using different kinds of operations (raise, lower, smooth)
 * and can also be painted with textures, even with different layers and 
 * smooth transitions (using texture splatting). Pages are aligned like
 * in an horizontal grid and are identified by integer coordinates.
 */

class VAPOR_API Terrain : public Geometry
{
public:

	Terrain();
	Terrain( resources::ImagePtr heightmap );
	
	~Terrain();

	// Creates a new page of terrain.
	Page* createPage( int x, int y );

	// Applies a brush operation in a specified region.
	//void applyBrush( const Brush& brush, const Region& region );

	// Gets the heightmap of the terrain.
	resources::ImagePtr getHeightmap() const;

	// Sets the heightmap of the terrain.
	void setHeightmap( resources::ImagePtr heightmap );

	// Updates the terrain geometry if needed.
	virtual void update( float delta );

	// Gets the type of this component. 
	virtual const std::string& getType() const;

protected:

	// Provides the heights of the terrain.
	resources::ImagePtr heightmap;

	// Pages of the terrain.
	std::vector< Page* > terrainPages;

	// Component type.
	static const std::string& type;
};

//-----------------------------------//

TYPEDEF_SHARED_POINTER_FROM_CLASS( Terrain );

//-----------------------------------//

} } // end namespaces
