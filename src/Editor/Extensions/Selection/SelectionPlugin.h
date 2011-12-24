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
	PluginMetadata getMetadata() OVERRIDE;

	// Plugin callbacks.
	void onPluginEnable() OVERRIDE;
	void onPluginDisable() OVERRIDE;

	// Plugin tool selection callback.
	void onToolSelect( int id ) OVERRIDE;
	void onToolNone() OVERRIDE;

	// Mouse callbacks.
	void onMouseButtonPress( const MouseButtonEvent& ) OVERRIDE;
	void onMouseButtonRelease( const MouseButtonEvent& ) OVERRIDE;
	void onMouseDrag( const MouseDragEvent& ) OVERRIDE;

	// Key callbacks.
	void onKeyPress(const KeyEvent&) OVERRIDE;
	void onKeyRelease(const KeyEvent&) OVERRIDE;

	// Document callbacks.
	void onDocumentSelect( Document& ) OVERRIDE;
	void onDocumentUnselect( Document& ) OVERRIDE;

	// Scene callbacks.
	void onSceneLoad( const ScenePtr& scene ) OVERRIDE;
	void onSceneUnload( const ScenePtr& scene ) OVERRIDE;

	// Entity callbacks.
	void onEntityRemoved(const EntityPtr& );
	void onEntityUnselect(const EntityPtr& ) OVERRIDE;

	// Undo callbacks.
	void onUndoOperation( const UndoOperationPtr& ) OVERRIDE;

	// Gets the entity picked by the ray.
	bool getPickEntity(int x, int y, EntityPtr& entity);

	// Gets the selection manager.
	SelectionManager* getSelectionManager() const;

public:

	// Creates the drag selection rectangle.
	EntityPtr createRectangle();

	// Updates the drag selection rectangle.
	void updateRectangle( const MouseDragEvent&, Entity* dragRectangle );

	// Creates a deselection operation.
	SelectionOperation* createDeselection();

	// Processes a normal selection.
	SelectionOperation* processSelection(const MouseButtonEvent& event);

	// Processes a drag selection.
	SelectionOperation* processDragSelection(const MouseButtonEvent& event);

	wxAuiToolBarItem* buttonSelect;

	Vector2 dragOrigin;
	SelectionManager* selections;

	// Holds if we are in additive mode.
	bool additiveMode;

	bool handlingEvents;
};

//-----------------------------------//

NAMESPACE_EDITOR_END