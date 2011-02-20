/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Scene/Geometry.h"

namespace vapor {

//-----------------------------------//

struct TerrainSettings;

/**
 * Cells are pieces of terrain geometry. They are further subdivided in 
 * cells, that are conceptually similar to tiles in 2D games, and allow
 * you to aligned like in an horizontal grid and are identified by
 * integer coordinates.
 */

class VAPOR_API Cell : public Geometry
{
	DECLARE_CLASS_()

public:

	Cell();

	Cell( int x, int y );

	// Sets the terrain settings of this terrain cell.
	void setSettings( const TerrainSettings& settings );

	// Gets the heights of this terrain cell.
	const std::vector<float>& getHeights();

	// Sets the heights of this terrain cell.
	void setHeights( const std::vector<float>& heights );

	// Rebuilds the terrain geometry.
	void rebuildGeometry();

	// Rebuilds the terrain normals.
	void rebuildNormals();

	// Given an indice, gets the neighbour vertices.
	byte getNeighborFaces( uint index, std::vector<uint>& n );

	// Gets the X coordinate of the cell.
	GETTER(X, int, x)

	// Gets the Y coordinate of the cell.
	GETTER(Y, int, y)

protected:

	// Calculate the vertices of the geometry.
	void rebuildVertices();

	// Calculate the indices of the geometry.
	void rebuildIndices();

	// Calculate the face normals of the geometry.
	void rebuildFaceNormals();

	// Calculate the averaged normals of the geometry.
	void rebuildAveragedNormals();

	// Coordinates of this cell of terrain.
	int x, y;

	// Terrain settings.
	const TerrainSettings* settings;

	// Stores the heights of this cell of terrain.
	std::vector<float> heights;
	
	// Stores the triangle face normals.
	std::vector<Vector3> faceNormals;

	// Cell image.
	ImagePtr image;

	// Cell material.
	MaterialPtr material;

	// Renderable of the cell.
	RenderablePtr rend;
};

TYPEDEF_SHARED_POINTER_FROM_TYPE( Cell );

//-----------------------------------//

} // end namespace