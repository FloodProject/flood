/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#ifdef ENABLE_PLUGIN_TERRAIN

#include "EditorPlugin.h"
#include "UndoOperation.h"
#include "TerrainPlugin.h"
#include "Math/BoundingSphere.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

class TerrainOperation : public UndoOperation
{
public:

	TerrainOperation( TerrainTool::Enum, const RayTriangleQueryResult& );
	
	void undo();
	void redo();
	void ready();

	void updateNormals();
	void loadSaveHeights( std::vector<float>& state, bool save );
	void loadSaveImage( std::vector<byte>& state, bool save );
	
	void applyTool();
	void applyRaiseTool();
	void applyPaintTool();

	void getCellsInRange(const BoundingSphere&, std::vector<CellPtr>& );
	void applyRaiseCell(const BoundingSphere&, const CellPtr& );

	TerrainPtr terrain;
	TerrainTool::Enum tool;
	RayTriangleQueryResult rayQuery;

	// Brush settings.
	int brushSize;
	int brushStrength;
	ImagePtr paintImage;
	ImagePtr brush;
	
	// Tile settings.
	bool tileLock;
	int tileOffsetX;
	int tileOffsetY;

	// Image data.
	std::vector<byte> beforeImage;
	std::vector<byte> afterImage;

	// Heights data.
	std::vector<float> beforeHeights;
	std::vector<float> afterHeights;
};

//-----------------------------------//

NAMESPACE_EDITOR_END

#endif