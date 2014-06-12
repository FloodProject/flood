/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Engine/Scene/Geometry.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

struct TerrainSettings;

/**
 * Cells are pieces of terrain geometry. They are further subdivided in 
 * cells, that are conceptually similar to tiles in 2D games, and allow
 * you to aligned like in an horizontal grid and are identified by
 * integer coordinates.
 */

API_ENGINE REFLECT_DECLARE_CLASS(Cell)

class API_ENGINE Cell : public Geometry
{
	REFLECT_DECLARE_OBJECT(Cell)

public:

	Cell();

	Cell( int32 x, int32 y );

	// Sets the terrain settings of this terrain cell.
	void setSettings( const TerrainSettings& settings );

	// Gets the heights of this terrain cell.
	const Vector<float>& getHeights();

	// Sets the heights of this terrain cell.
	void setHeights( const Vector<float>& heights );

	// Rebuilds the terrain geometry.
	void rebuildGeometry();

	// Rebuilds the terrain normals.
	void rebuildNormals();

	// Given an indice, gets the neighbour vertices.
	byte getNeighborFaces( uint index, Vector<uint>& n );

	// Gets the X coordinate of the cell.
	GETTER(X, int32, x)

	// Gets the Y coordinate of the cell.
	GETTER(Y, int32, y)

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
	int32 x;
	int32 y;

	// Terrain settings.
	const TerrainSettings* settings;

	// Stores the heights of this cell of terrain.
	Vector<float> heights;
	
	// Stores the triangle face normals.
	Vector<Vector3> faceNormals;

	// Cell image.
	ImageHandle image;

	// Cell material.
	MaterialHandle material;

	// Renderable of the cell.
	RenderBatchPtr rend;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Cell );

typedef Cell TerrainCell;
typedef CellPtr TerrainCellPtr;

//-----------------------------------//

NAMESPACE_ENGINE_END