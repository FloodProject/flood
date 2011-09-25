/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Editor/API.h"
#include "Editor.h"

#include "Document.h"
#include "DocumentWindow.h"

#include "UndoManager.h"
#include "EventManager.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

Document::Document()
	: unsavedChanges(false)
	, undoManager(nullptr)
	, documentWindow(nullptr)
{
}

//-----------------------------------//

Document::~Document()
{
	EventManager* events = GetEditor().getEventManager();
	events->onDocumentDestroy(*this);

	resetUndo();
}

//-----------------------------------//

void Document::setPath(const Path& newPath)
{
	path = newPath;
	GetEditor().getDocumentManager()->onDocumentRenamed(this);
}

//-----------------------------------//

void Document::create()
{
	assert( !undoManager );
	createUndo();

	assert( !documentWindow );
	documentWindow = createDocumentWindow();

	reset();
}

//-----------------------------------//

DocumentBar* Document::createDocumentBar()
{
	DocumentBar* bar = new wxAuiToolBar( (wxWindow*) getWindow() );
	bar->Realize();

	getWindow()->mainSizer->Add(bar, wxSizerFlags().Expand().Top());

	return bar;
}

//-----------------------------------//

void Document::createUndo()
{
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