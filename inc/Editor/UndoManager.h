/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Event.h"
#include "Core/References.h"
#include "UndoOperation.h"
#include <deque>

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

class UndoOperation;
typedef std::deque<UndoOperationPtr> UndoOperations;

/**
 * Manages undo/redo operations.
 */

class UndoManager
{
public:

	~UndoManager();

	// Registers a new operation.
	void registerOperation( UndoOperation* operation );

	// Undoes an operation.
	bool undoOperation();

	// Redoes an operation.
	bool redoOperation();

	// Clears the operations.
	void clearOperations();

	// Gets the undo operations.
	GETTER(UndoOperations, const UndoOperations&, undoOperations)

	// Gets the redo operations.
	GETTER(RedoOperations, const UndoOperations&, redoOperations)

	// Gets sent on an undo/redo event.
	Event0<> onUndoRedoEvent;

protected:

	// Sends the undo/redo event.
	void sendUndoRedoEvent();

	// Handles undo/redo operations.
	void handleOperation(UndoOperations& first,	UndoOperations& second, bool undo);

	// Undo operations.
	UndoOperations undoOperations;

	// Redo operations.
	UndoOperations redoOperations;
};

//-----------------------------------//

NAMESPACE_EDITOR_END