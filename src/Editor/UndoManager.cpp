/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "PCH.h"
#include "UndoManager.h"
#include "UndoOperation.h"
#include "Editor.h"

namespace vapor { namespace editor {

//-----------------------------------//

UndoManager::~UndoManager()
{
	for( uint i = 0; i < undoOperations.size(); i++ )
		delete undoOperations[i];

	for( uint i = 0; i < redoOperations.size(); i++ )
		delete redoOperations[i];
}

//-----------------------------------//

void UndoManager::registerOperation( UndoOperation* operation )
{
	undoOperations.push_back( operation );

	Log::debug("Registered undo/redo event: %s", operation->description.c_str() );

	// Remove all the existing redo operations.
	for( uint i = 0; i < redoOperations.size(); i++ )
		delete redoOperations[i];

	redoOperations.clear();
	onUndoRedoEvent();
}

//-----------------------------------//

void UndoManager::clearOperations( )
{
	undoOperations.clear();
	redoOperations.clear();

	onUndoRedoEvent();
}

//-----------------------------------//

bool UndoManager::undoOperation()
{
	handleOperation(undoOperations, redoOperations, true);
	onUndoRedoEvent();

	return undoOperations.empty();
}

//-----------------------------------//

bool UndoManager::redoOperation()
{
	handleOperation(redoOperations, undoOperations, false);
	onUndoRedoEvent();

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

	GetEditor().redrawView();
}

//-----------------------------------//

} } // end namespaces