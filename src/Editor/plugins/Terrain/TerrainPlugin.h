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
#include "Scene/Scene.h"
#include "Terrain/Terrain.h"

namespace vapor { namespace editor {

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

class TerrainPage;
class TerrainOperation;

REFLECT_DECLARE_CLASS(TerrainPlugin)

class TerrainPlugin : public EditorPlugin
{
	REFLECT_DECLARE_OBJECT(TerrainPlugin)

public:

	TerrainPlugin();

	// Gets metadata about this plugin.
	virtual PluginMetadata getMetadata();

	virtual void onPluginEnable();
	virtual void onPluginDisable();
	
	// Plugin tool selection callback.
	virtual void onToolSelect( int id );

	// Plugin tool unselection callback.
	virtual void onToolUnselect( int id );

	// Entity callbacks.
	virtual void onEntitySelect( const EntityPtr& );
	virtual void onEntityUnselect( const EntityPtr& );

	// Mouse callbacks.
	virtual void onMouseMove( const MouseMoveEvent& );
	virtual void onMouseDrag( const MouseDragEvent& );
	virtual void onMouseButtonPress( const MouseButtonEvent& );
	virtual void onMouseButtonRelease( const MouseButtonEvent& );
	virtual void onMouseLeave();

protected:

	// Callback used when editing the terrain by holding the mouse button.
	void onTimer( wxTimerEvent& event );

	// Callback when cells need to created.
	void onCreateCell( wxCommandEvent& event );

	// Callback when cell normals need to be rebuilt.
	void onRebuildNormals( wxCommandEvent& event );

	// Ray picks the terrain and returns the cell coords.
	bool pickCell( int x, int y );

	// Ray picks the terrain and returns the intersection point.
	bool pickTerrain( int x, int y, RayTriangleQueryResult& res );

	// Projects a brush unto the terrain.
	void projectBrush(const Vector3& pos, const TerrainCellPtr& cell );

	// Updates the brush projection unto the terrain.
	void updateBrushProjection( int x, int y );

	// Creates a new projector to project the brush.
	EntityPtr createProjector(const GeometryPtr& cell);

	// Handles the right-click context menu creation.
	void createContextMenu( const MouseButtonEvent& mbe );

	// Sets up the terrain operation.
	void setupOperation( const MouseButtonEvent& mb );

	// Creates a new operation if there is none currently.
	void createOperation( const RayTriangleQueryResult& res );

	// Registers the operation in undo history.
	void registerUndoOperation();

	// Terrain notebook page.
	TerrainPtr terrain;

	// Terrain notebook page.
	TerrainPage* terrainPage;

	// Holds the current terrain operation.
	TerrainOperation* terrainOperation;

	// Current cell cordinates.
	Vector2i coords;
	
	// Callback used when editing the terrain by holding the mouse button.
	wxTimer timer;

	// Holds the current projector.
	EntityPtr entityProjector;

	// Current tool.
	TerrainTool::Enum tool;

	// Toolbar buttons.
	wxAuiToolBarItem* buttonRaise;
	wxAuiToolBarItem* buttonPaint;
	
	int32 iconTerrain;
};

//-----------------------------------//

} } // end namespaces

#endif