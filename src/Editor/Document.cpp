/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Editor/API.h"

#include "Editor.h"
#include "EditorIcons.h"
#include "ImageUtils.h"

#include "Document.h"
#include "DocumentWindow.h"

#include "UndoManager.h"
#include "EventManager.h"
#include "Extensions/UndoRedo/UndoPlugin.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

REFLECT_ABSTRACT_CHILD_CLASS(Document, Object)
REFLECT_CLASS_END()

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

wxBitmap Document::getBitmap()
{
	return wxMEMORY_BITMAP(package);
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

bool Document::reset()
{
	undoManager->clearOperations();
	unsavedChanges = false;

	return onDocumentReset();
}

//-----------------------------------//

DocumentBar* Document::createDocumentBar()
{
	DocumentBar* bar = new wxAuiToolBar( getWindow() );
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

	setUnsavedChanges(false);
}

//-----------------------------------//

void Document::setUnsavedChanges(bool newUnsavedChanges)
{
	unsavedChanges = newUnsavedChanges;
	GetEditor().getDocumentManager()->onDocumentRenamed(this);
}

//-----------------------------------//

void Document::onUndoRedoEvent()
{
	bool unsavedChanges = !getUndoManager()->getUndoOperations().empty();
	setUnsavedChanges(unsavedChanges);
}

//-----------------------------------//

NAMESPACE_EDITOR_END