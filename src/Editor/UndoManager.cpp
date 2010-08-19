/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "PCH.h"
#include "UndoManager.h"

namespace vapor { namespace editor {

//-----------------------------------//

UndoManager::~UndoManager()
{
	foreach( UndoOperation* const op, undoOperations )
		delete op;

	foreach( UndoOperation* const op, redoOperations )
		delete op;
}

//-----------------------------------//

void UndoManager::registerOperation( UndoOperation* operation )
{
	undoOperations.push_back( operation );

	// Remove all the existing redo operations.
	foreach( UndoOperation* const op, redoOperations )
		delete op;

	redoOperations.clear();

	fireUndoRedoEvent();
}

//-----------------------------------//

void UndoManager::clearOperations( )
{
	undoOperations.clear();
	redoOperations.clear();

	fireUndoRedoEvent();
}

//-----------------------------------//

bool UndoManager::undoOperation()
{
	handleOperation(undoOperations, redoOperations, true);
	fireUndoRedoEvent();

	return undoOperations.empty();
}

//-----------------------------------//

bool UndoManager::redoOperation()
{
	handleOperation(redoOperations, undoOperations, false);
	fireUndoRedoEvent();

	return redoOperations.empty();
}

//-----------------------------------//

void UndoManager::handleOperation(Operations& firstOperations,
								  Operations& secondOperations,
								  bool undo)
{
	if( firstOperations.empty() )
		return;

	UndoOperation* op = firstOperations.back();

	if(!op)
		return;

	firstOperations.pop_back();
	secondOperations.push_back(op);

	if(undo)
		op->undo();
	else
		op->redo();
}

//-----------------------------------//

void UndoManager::fireUndoRedoEvent()
{
	if( !onUndoRedoEvent.empty() )
		onUndoRedoEvent();
}

//-----------------------------------//

} } // end namespaces