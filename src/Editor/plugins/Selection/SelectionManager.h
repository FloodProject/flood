/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "UndoOperation.h"

namespace vapor { namespace editor {

//-----------------------------------//

class EditorFrame;
class UndoManager;
class Events;
class SelectionManager;

//-----------------------------------//

namespace SelectionMode
{
	enum Enum
	{
		None,
		Vertex,
		Edge,
		Face,
		Entity,
		Group
	};
}

//-----------------------------------//

struct SelectionData
{
	bool operator == (const SelectionData& rhs) const;
	EntityPtr entity;
};

class SelectionOperation : public UndoOperation
{
	friend class SelectionManager;

public:

	~SelectionOperation();

	// Undoes the operation.
	virtual void undo();	

	// Redoes the operation.
	virtual void redo();

	// Adds an entity to the selection.
	void addEntity(const EntityPtr& entity);

	// Checks if an entity is in the selection.
	bool isSelection(const EntityPtr& entity);

	// Selects all the objects.
	void selectAll();

	// Unselects all the objects.
	void unselectAll();

	// Selects all the previous selected objects.
	void selectPrevious();

	// Unselects all the previous selected objects.
	void unselectPrevious();

	// Selection mode.
	SelectionMode::Enum mode;

	// Selections.
	std::vector<SelectionData> selections;
	std::vector<SelectionData> previous;

protected:

	// Sets the bounding box visibility of the given node.
	void setBoundingBoxVisible( const EntityPtr& node, bool state );

	SelectionManager* selectionManager;
};

/**
 * Manages and keeps track of selections, which can be of different modes.
 * You can also make group selections by using a drag select operation.
 * This also makes sure that selections are treated as first-class operations
 * in the editor undo/redo system.
 */

class SelectionManager
{
public:

	SelectionManager();

	// Gets the current selection mode.
	SelectionMode::Enum getSelectionMode() const;

	// Sets the current selection mode.
	void setSelectionMode(SelectionMode::Enum mode);

	// Gets the current selection.
	SelectionOperation* getSelection() const;

	// Sets the current selection.
	void setSelection(SelectionOperation* selection);

	// Creates a new selection operation.
	SelectionOperation* createOperation();

protected:

	// Selection mode.
	SelectionMode::Enum mode;

	// Selection operation.
	SelectionOperation* selection;
};

//-----------------------------------//

} } // end namespaces