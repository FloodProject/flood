/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Plugin.h"
#include "Math/Vector.h"

namespace vapor { namespace editor {

//-----------------------------------//

class SelectionManager;
class SelectionOperation;

REFLECT_DECLARE_CLASS(SelectionPlugin)

class SelectionPlugin : public Plugin
{
	REFLECT_DECLARE_OBJECT(SelectionPlugin)

public:

	SelectionPlugin();

	// Gets metadata about this plugin.
	virtual PluginMetadata getMetadata();

	// Plugin enable callback.
	virtual void onPluginEnable();

	// Plugin disable callback.
	virtual void onPluginDisable();

	// Scene load callback.
	void onSceneLoad( const ScenePtr& scene );

	// Scene entity removed callback.
	void onEntityRemoved(const EntityPtr& entity);

	// Mouse button press callback.
	virtual void onMouseButtonPress( const MouseButtonEvent& );

	// Mouse button release callback.
	virtual void onMouseButtonRelease( const MouseButtonEvent& );

	// Mouse drag callback.
	virtual void onMouseDrag( const MouseDragEvent& );

	// Gets the entity picked by the ray.
	bool getPickEntity(int x, int y, EntityPtr& entity);

	// Gets the selection manager.
	SelectionManager* getSelectionManager() const;

protected:

	// Creates the drag selection rectangle.
	void createRectangle();

	// Updates the drag selection rectangle.
	void updateRectangle( const MouseDragEvent& );

	// Creates a deselection operation.
	SelectionOperation* createDeselection();

	// Processes a normal selection.
	SelectionOperation* processSelection(const MouseButtonEvent& event);

	// Processes a drag selection.
	SelectionOperation* processDragSelection(const MouseButtonEvent& event);

	SelectionManager* selections;
	wxAuiToolBarItem* buttonSelect;

	Vector2 dragOrigin;
	EntityPtr dragRectangle;
};

//-----------------------------------//

} } // end namespaces