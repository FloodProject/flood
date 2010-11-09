/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "UndoOperation.h"

namespace vapor { namespace editor {

typedef std::deque<UndoOperation*> Operations;

//-----------------------------------//

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
	GETTER(UndoOperations, const Operations&, undoOperations)

	// Gets the redo operations.
	GETTER(RedoOperations, const Operations&, redoOperations)

	// Gets sent on an undo/redo event.
	Event0<> onUndoRedoEvent;

protected:

	// Fires the undo/redo event.
	void fireUndoRedoEvent();

	// Handles undo/redo operations.
	void handleOperation(Operations& firstOperations,
		Operations& secondOperations, bool undo);

	// Undo operations.
	Operations undoOperations;

	// Redo operations.
	Operations redoOperations;
};



//-----------------------------------//

} } // end namespaces