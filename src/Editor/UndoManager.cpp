/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Editor/API.h"
#include "UndoManager.h"
#include "UndoOperation.h"
#include "Editor.h"
#include "EventManager.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

UndoManager::~UndoManager()
{
	clearOperations();
}

//-----------------------------------//

void UndoManager::registerOperation( UndoOperation* operation )
{
	if( !operation ) return;

	if( operation->description.empty() )
		LogAssert("Undo operation description should not be empty");

	undoOperations.push_back( operation );

	LogDebug("Registered undo/redo event: %s", operation->description.c_str() );

	// Remove all the existing redo operations.
	for( size_t i = 0; i < redoOperations.size(); i++ )
	{
		const UndoOperationPtr& op = redoOperations[i];
		
		if( op.get()->references != 1)
			LogAssert("Undo operation '%s' should not have any references", op->getType()->name);
	}

	redoOperations.clear();

	onUndoRedoEvent();
}

//-----------------------------------//

void UndoManager::clearOperations()
{
	undoOperations.clear();
	redoOperations.clear();

	//onUndoRedoEvent();
}

//-----------------------------------//

bool UndoManager::undoOperation()
{
	if( undoOperations.empty() ) return false;

	EventManager* events = GetEditor().getEventManager();
	events->onUndoOperation( undoOperations.front() );

	handleOperation(undoOperations, redoOperations, true);
	onUndoRedoEvent();

	return true;
}

//-----------------------------------//

bool UndoManager::redoOperation()
{
	if( redoOperations.empty() ) return false;

	EventManager* events = GetEditor().getEventManager();
	events->onRedoOperation( redoOperations.front() );

	handleOperation(redoOperations, undoOperations, false);
	onUndoRedoEvent();

	return true;
}

//-----------------------------------//

void UndoManager::handleOperation(UndoOperations& firstOperations,
	UndoOperations& secondOperations, bool undo)
{
	if( firstOperations.empty() )
		return;

	UndoOperationPtr op = firstOperations.back();
	if(!op) return;

	firstOperations.pop_back();
	secondOperations.push_back(op);

	if(undo)
		op->undo();
	else
		op->redo();
}

//-----------------------------------//

NAMESPACE_EDITOR_END