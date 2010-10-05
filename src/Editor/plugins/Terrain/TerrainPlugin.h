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

namespace vapor { namespace editor {

//-----------------------------------//

class TerrainPage;

//-----------------------------------//

namespace TerrainTool
{
	enum Enum
	{
		Raise,
		Lower,
		Smooth,
		Noise,
		Paint
	};
}

//-----------------------------------//

class TerrainOperation : public UndoOperation
{
public:

	TerrainOperation( TerrainTool::Enum tool,
		const RayTriangleQueryResult& res );
	
	void undo();
	void redo();
	void ready();

	void updateNormals();
	void loadSaveHeights( std::vector<float>& state, bool save );
	void loadSaveImage( std::vector<byte>& state, bool save );
	
	void applyTool();
	void applyRaiseTool();
	void applyPaintTool();

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

class TerrainPlugin : public wxEvtHandler, public Plugin
{
public:

	TerrainPlugin( EditorFrame* frame );

	// Gets metadata about this plugin.
	virtual PluginMetadata getMetadata();

	virtual void onPluginEnable();
	virtual void onPluginDisable();
	
	// Node callbacks.
	virtual void onNodeSelect( const NodePtr& );
	virtual void onNodeUnselect( const NodePtr& );

	// Mouse callbacks.
	virtual void onMouseDrag( const MouseDragEvent& mde );
	virtual void onMouseButtonPress( const MouseButtonEvent& mbe );
	virtual void onMouseButtonRelease( const MouseButtonEvent& );
	virtual void onMouseLeave();

protected:

	// Handles tools buttons click.
	void onToolClick(wxCommandEvent& event);

	// Callback used when editing the terrain by holding the mouse button.
	void onTimer( wxTimerEvent& event );

	// Callback when cells need to created.
	void onCreateCell( wxCommandEvent& event );

	// Callback when cell normals need to be rebuilt.
	void onRebuildNormals( wxCommandEvent& event );

	// Ray picks the terrain and returns the cell coords.
	bool pickCell( const MouseButtonEvent& mbe );

	// Ray picks the terrain and returns the intersection point.
	bool pickTerrain( const MouseButtonEvent& mb, RayTriangleQueryResult& res );

	// Handles the right-click context menu creation.
	void createContextMenu( const MouseButtonEvent& mbe );

	// Sets up the terrain operation.
	void setupOperation( const MouseButtonEvent& mb );

	// Creates a new operation if there is none currently.
	void createOperation( const RayTriangleQueryResult& res );

	// Finishes the operation and registers the event in the history stack.
	void registerEvent();

	// Terrain notebook page.
	TerrainPtr terrain;

	// Terrain notebook page.
	TerrainPage* terrainPage;

	// Holds the current terrain operation.
	TerrainOperation* op;

	// Current cell cordinates.
	Vector2i coords;
	
	// Callback used when editing the terrain by holding the mouse button.
	wxTimer timer;

	// Current tool.
	TerrainTool::Enum tool;

	// Toolbar buttons.
	wxToolBarToolBase* buttonRaise;
	wxToolBarToolBase* buttonPaint;

private:

    DECLARE_EVENT_TABLE()
};

//-----------------------------------//

} } // end namespaces