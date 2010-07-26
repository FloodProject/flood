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

protected:

	// Handles undo/redo operations.
	void handleOperation(bool undo);

	// Undo operations.
	Operations undoOperations;

	// Redo operations.
	Operations redoOperations;
};



//-----------------------------------//

} } // end namespaces