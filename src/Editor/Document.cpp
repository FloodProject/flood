/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Editor/API.h"
#include "Document.h"
#include "UndoManager.h"
#include "Editor.h"
#include "EventManager.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

Document::Document()
	: unsavedChanges(false)
	, undoManager(nullptr)
	, name("(untitled)")
{
	createUndo();
}

//-----------------------------------//

Document::~Document()
{
	EventManager* events = GetEditor().getEventManager();
	events->onDocumentDestroy(*this);

	resetUndo();
}

//-----------------------------------//

void Document::createUndo()
{
	assert( !undoManager );

	undoManager = AllocateThis(UndoManager);
	undoManager->onUndoRedoEvent.Connect(this, &Document::onUndoRedoEvent);
}

//-----------------------------------//

void Document::resetUndo()
{
	if( !undoManager) return;

	undoManager->onUndoRedoEvent.Disconnect(this, &Document::onUndoRedoEvent);
	Deallocate(undoManager);

	unsavedChanges = false;
}

//-----------------------------------//

void Document::onUndoRedoEvent()
{
	unsavedChanges = true;
}

//-----------------------------------//

NAMESPACE_EDITOR_END