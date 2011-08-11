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

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

Document::Document()
	: unsavedChanges(false)
	, name("(untitled)")
{
	undoManager = Allocate(UndoManager, AllocatorGetThis());
	undoManager->onUndoRedoEvent.Connect(this, &Document::onUndoRedoEvent);
}

//-----------------------------------//

Document::~Document()
{
	undoManager->onUndoRedoEvent.Disconnect(this, &Document::onUndoRedoEvent);
	Deallocate(undoManager);
}

//-----------------------------------//

void Document::onUndoRedoEvent()
{
	unsavedChanges = true;
}

//-----------------------------------//

NAMESPACE_EDITOR_END