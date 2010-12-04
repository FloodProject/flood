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
	for( uint i = 0; i < undoOperations.size(); i++ )
	{
		UndoOperation* op = undoOperations[i];
		delete op;
	}

	for( uint i = 0; i < redoOperations.size(); i++ )
	{
		UndoOperation* op = redoOperations[i];
		delete op;
	}
}

//-----------------------------------//

void UndoManager::registerOperation( UndoOperation* operation )
{
	undoOperations.push_back( operation );

	// Remove all the existing redo operations.
	for( uint i = 0; i < redoOperations.size(); i++ )
	{
		UndoOperation* op = redoOperations[i];
		delete op;
	}

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
	onUndoRedoEvent();
}

//-----------------------------------//

} } // end namespaces