/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "UndoOperation.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

class EditorFrame;
class UndoManager;
class EventManager;
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
	};
}

//-----------------------------------//

struct SelectionData
{
	bool operator == (const SelectionData& rhs) const;
	EntityPtr entity;
};

REFLECT_DECLARE_CLASS(SelectionOperation)

class SelectionOperation : public UndoOperation
{
	REFLECT_DECLARE_OBJECT(SelectionOperation)
	friend class SelectionManager;

public:

	~SelectionOperation();

	// Undoes/redoes the operation.
	void undo() OVERRIDE;	
	void redo() OVERRIDE;

	// Selects/unselects all the objects.
	void selectAll();
	void unselectAll();

	// Selects/unselects all the previous selected objects.
	void selectPrevious();
	void unselectPrevious();

	// Adds an entity to the selection.
	void addEntity(const EntityPtr& entity);

	// Sets the previous selections.
	void setPreviousSelections(SelectionOperation*);

	// Checks if an entity is in the selection.
	bool isSelection(const EntityPtr& entity);

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

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE(SelectionOperation)

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
	SelectionOperation* createOperation( SelectionMode::Enum );

protected:

	// Selection mode.
	SelectionMode::Enum mode;

	// Selection operation.
	SelectionOperationPtr selection;
};

//-----------------------------------//

NAMESPACE_EDITOR_END