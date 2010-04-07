/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/render/Renderable.h"

namespace vapor { namespace scene {

struct TerrainSettings;

//-----------------------------------//

/**
 * Cells are pieces of terrain geometry. They are further subdivided in 
 * cells, that are conceptually similar to tiles in 2D games, and allow
 * you to aligned like in an horizontal grid and are identified by
 * integer coordinates.
 */

class VAPOR_API Cell : public render::Renderable, private boost::noncopyable
{
public:

	Cell( const TerrainSettings& settings, 
		const std::vector<float>& heights, ushort x, ushort y );

	// Gets the heights of this Cell of terrain.
	const std::vector<float>& getHeights();

	// Sets the heights of this Cell of terrain.
	void setHeights( const std::vector<float>& heights );

	// Generates the terrain grid geometry.
	void updateGeometry();

	// Calculate the normals of the geometry.
	void calculateNormals( const std::vector<math::Vector3>& vertices );

	// Given an indice, gets the neighbour vertices.
	std::vector<uint> getNeighborVertices( uint index );

protected:

	// Coordinates of this cell of terrain.
	ushort x, y;

	// Stores the heights of this cell of terrain.
	std::vector<float> heights;
	
	// Terrain settings.
	const TerrainSettings& settings;
};

//-----------------------------------//

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Cell );

//-----------------------------------//

} } // end namespaces
