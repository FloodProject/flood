/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Document.h"
#include "UndoOperation.h"

FWD_DECL_INTRUSIVE(SelectionOperation)

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

namespace SelectionMode
{
	enum Enum
	{
		None,
		Entity,
		Face,
		Edge,
		Vertex,
	};
}

//-----------------------------------//

struct SelectionData
{
	SelectionData();

	bool operator == (const SelectionData& rhs) const;
	
	SelectionMode::Enum mode;
	EntityPtr entity;
};

typedef std::vector<SelectionData> SelectionsVector;

struct SelectionCollection
{
	// Returns if there are no selections.
	bool isEmpty() const;

	// Adds an entity to the selection.
	void addEntity(const EntityPtr& entity);

	// Checks if an entity is in the selection.
	bool hasEntity(const EntityPtr& entity) const;

	// Returns true if the collection contain exactly the same objects.
	bool isSame(const SelectionCollection&) const;

	// Returns true if selection is contained.
	bool contains(const SelectionData&) const;

	// Returns true if selection is contained.
	bool contains(const SelectionCollection&) const;

	SelectionsVector selections;
};

//-----------------------------------//

/**
 * Manages and keeps track of selections, which can be of different modes.
 * This also makes sure that selections are treated as first-class operations
 * in the editor undo/redo system.
 */

class SelectionManager : public DocumentContext
{
public:

	SelectionManager();

	// Gets the current selection mode.
	SelectionMode::Enum getSelectionMode() const;

	// Sets the current selection mode.
	void setSelectionMode(SelectionMode::Enum mode);

	// Gets the current selections.
	const SelectionCollection& getSelections() const;

	// Adds the selection to the current selections.
	void addSelection(const SelectionData&);

	// Removes the selection from the current selections.
	void removeSelection(const SelectionData&);

	// Adds the selections to the current selections.
	void addSelections(const SelectionCollection&);

	// Removes the selections to the current selections.
	void removeSelections(const SelectionCollection&);

	// Removes all the current selections.
	void removeCurrent();

	EntityPtr dragRectangle;

protected:

	// Selection mode.
	SelectionMode::Enum mode;

	// Current selections.
	SelectionCollection selections;
};

//-----------------------------------//

NAMESPACE_EDITOR_END