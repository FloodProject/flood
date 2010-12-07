/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

namespace vapor { namespace editor {

//-----------------------------------//

class UndoOperation;
typedef std::deque<UndoOperation*> Operations;

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
	GETTER(UndoOperations, const Operations&, undoOperations)

	// Gets the redo operations.
	GETTER(RedoOperations, const Operations&, redoOperations)

	// Gets sent on an undo/redo event.
	Event0<> onUndoRedoEvent;

protected:

	// Sends the undo/redo event.
	void sendUndoRedoEvent();

	// Handles undo/redo operations.
	void handleOperation(Operations& first,	Operations& second, bool undo);

	// Undo operations.
	Operations undoOperations;

	// Redo operations.
	Operations redoOperations;
};

//-----------------------------------//

} } // end namespaces