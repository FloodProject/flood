/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Plugin.h"
#include "UndoOperation.h"
#include "TerrainPlugin.h"

namespace vapor { namespace editor {

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

} } // end namespaces