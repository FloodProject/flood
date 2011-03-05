/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

class wxWindow;

namespace vapor { namespace editor {

//-----------------------------------//

class UndoManager;

class Document
{
public:

	Document();
	virtual ~Document();

	// Gets the name of the document.
	GETTER(Name, const std::string&, name)

	// Gets the undo manager of the document.
	GETTER(UndoManager, UndoManager*, undoManager)

	// Gets if the document has unsaved changes.
	GETTER(UnsavedChanges, bool, unsavedChanges)

	// Gets the document notebook window.
	virtual wxWindow* getWindow() = 0;

	// Load and saving of documents.

	// Document selection callbacks.
	virtual void onDocumentSelect() {}
	virtual void onDocumentUnselect() {}

protected:

	// Handles undo/redo events.
	void onUndoRedoEvent();

	std::string name;
	bool unsavedChanges;
	UndoManager* undoManager;
};

//-----------------------------------//

} } // end namespaces