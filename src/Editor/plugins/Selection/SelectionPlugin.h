/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "EditorPlugin.h"
#include "Math/Vector.h"

#define PLUGIN_SELECTION "Selection"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

class SelectionManager;
class SelectionOperation;

REFLECT_DECLARE_CLASS(SelectionPlugin)

class SelectionPlugin : public EditorPlugin
{
	REFLECT_DECLARE_OBJECT(SelectionPlugin)

public:

	SelectionPlugin();

	// Gets metadata about this plugin.
	virtual PluginMetadata getMetadata() OVERRIDE;

	// Plugin tool selection callback.
	virtual void onToolSelect( int id ) OVERRIDE;

	// Plugin enable callback.
	virtual void onPluginEnable() OVERRIDE;

	// Plugin disable callback.
	virtual void onPluginDisable() OVERRIDE;

	// Document creation callback.
	virtual void onDocumentCreate( Document& document ) OVERRIDE;
	//virtual void onDocumentDestroy( Document& document ) {}

	// Scene load callback.
	void onSceneLoad( const ScenePtr& scene ) OVERRIDE;

	// Mouse button press callback.
	virtual void onMouseButtonPress( const MouseButtonEvent& ) OVERRIDE;

	// Mouse button release callback.
	virtual void onMouseButtonRelease( const MouseButtonEvent& ) OVERRIDE;

	// Mouse drag callback.
	virtual void onMouseDrag( const MouseDragEvent& ) OVERRIDE;

	// Key callback.
	virtual void onKeyPress(const KeyEvent&) OVERRIDE;
	virtual void onKeyRelease(const KeyEvent&) OVERRIDE;

	// Scene entity removed callback.
	void onEntityRemoved(const EntityPtr& entity);

	// Gets the entity picked by the ray.
	bool getPickEntity(int x, int y, EntityPtr& entity);

	// Gets the selection manager.
	SelectionManager* getSelectionManager() const;

public:

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

	// Holds if we are in additive mode.
	bool additiveMode;
};

//-----------------------------------//

NAMESPACE_EDITOR_END