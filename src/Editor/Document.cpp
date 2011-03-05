/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "PCH.h"
#include "Document.h"
#include "UndoManager.h"

namespace vapor { namespace editor {

//-----------------------------------//

Document::Document()
	: unsavedChanges(false)
	, name("(untitled)")
{
	undoManager = new UndoManager();
	undoManager->onUndoRedoEvent.Connect(this, &Document::onUndoRedoEvent);
}

//-----------------------------------//

Document::~Document()
{
	undoManager->onUndoRedoEvent.Disconnect(this, &Document::onUndoRedoEvent);
	delete undoManager;
}

//-----------------------------------//

void Document::onUndoRedoEvent()
{
	unsavedChanges = true;
}

//-----------------------------------//

} } // end namespaces