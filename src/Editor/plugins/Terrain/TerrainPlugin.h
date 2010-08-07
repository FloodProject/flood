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
		Paint,
		Smooth
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
	void processState( std::vector<float>& state, bool save );
	void applyTerrainTool();

	float size;
	float strength;
	TerrainTool::Enum tool;
	RayTriangleQueryResult res;

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

	// Callback used when editing the terrain by holding the mouse button.
	void onTimer( wxTimerEvent& event );

	// Callback when cells need to created.
	void onCreateCell( wxCommandEvent& event );

	// Ray picks the terrain and returns the intersection point.
	bool pickTerrain( const MouseButtonEvent& mb, RayTriangleQueryResult& res );

	// Checks if the ray hits and deforms.
	void deformTerrain( const MouseButtonEvent& mb );

	// Creates a new operation if there is none currently.
	void createOperation( const RayTriangleQueryResult& res );

	// Finishes the operation and registers the event in the history stack.
	void registerEvent();

protected:

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

private:

    DECLARE_EVENT_TABLE()
};

//-----------------------------------//

} } // end namespaces