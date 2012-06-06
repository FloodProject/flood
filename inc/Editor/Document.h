/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Core/Object.h"
#include <wx/toolbar.h>

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

class DocumentWindow;
typedef wxAuiToolBar DocumentBar;

/** Using the document context map you can store plugin-specific data. */

struct DocumentContext : public Object
{
};

typedef std::map<void*, RefPtr<DocumentContext>> DocumentContextMap;

/**
 * Documents are the basis of the editing functionality. They allow the
 * editor to provide a customized interface for each kind of resource.
 */

class UndoManager;
class ToolExtension;

REFLECT_DECLARE_CLASS(Document)

class Document : public Object
{
	REFLECT_DECLARE_OBJECT(Document)

public:

	Document();
	virtual ~Document();

	// Gets the path to the document.
	GETTER(Path, const Path&, path)

	// Sets the path to the document.
	void setPath(const Path&);

	// Gets if the document has unsaved changes.
	GETTER(UnsavedChanges, bool, unsavedChanges)

	// Sets if the document has unsaved changes.
	void setUnsavedChanges(bool unsavedChanges);

	// Creates and resets the document.
	void create();

	// Resets the document to a clean state.
	bool reset();

	// Document management callbacks.
	virtual bool onDocumentOpen() = 0;
	virtual bool onDocumentSave() = 0;
	virtual bool onDocumentReset() = 0;
	virtual void onDocumentDestroy() = 0;

	// Document selection callbacks.
	virtual void onDocumentSelect() {}
	virtual void onDocumentUnselect() {}

	// Creates the undo management state.
	void createUndo();

	// Resets the undo management state.
	void resetUndo();

	// Gets the undo manager of the document.
	GETTER(UndoManager, UndoManager*, undoManager)

	// Gets the document notebook window.
	GETTER(Window, DocumentWindow*, documentWindow)

	// Creates the document window.
	virtual DocumentWindow* createDocumentWindow() = 0;

	// Creates a context toolbar.
	virtual DocumentBar* createDocumentBar();

	// Tool selection callbacks.
	virtual void onToolSelect(ToolExtension* tool) {}

	// File dialog description.
	virtual const char* getFileDialogDescription() = 0;

	// Document bitmap icon.
	virtual wxBitmap getBitmap();

	// Keeps the context data of the document.
	DocumentContextMap documentContext;

protected:

	// Handles undo/redo events.
	void onUndoRedoEvent();

	// Path to the document. 
	Path path;

	// Window representing the document.
	DocumentWindow* documentWindow;

	// Keeps the undo command stacks.
	UndoManager* undoManager;

	// Keeps if the document contains unsaved changes.
	bool unsavedChanges;
};

//-----------------------------------//

NAMESPACE_EDITOR_END