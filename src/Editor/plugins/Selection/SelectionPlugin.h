/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Plugin.h"

namespace vapor { namespace editor {

//-----------------------------------//

class SelectionManager;

class SelectionPlugin : public Plugin
{
public:

	SelectionPlugin( EditorFrame* frame );

	// Gets metadata about this plugin.
	virtual PluginMetadata getMetadata();

	// Plugin enable callback.
	virtual void onPluginEnable();

	// Plugin disable callback.
	virtual void onPluginDisable();

	// Scene load callback.
	void onSceneLoad( const ScenePtr& scene );

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

	SelectionManager* selections;
	wxToolBarToolBase* buttonSelect;

	Vector2i dragOrigin;
	EntityPtr dragRectangle;
};

//-----------------------------------//

} } // end namespaces