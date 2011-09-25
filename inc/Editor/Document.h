/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

class DocumentWindow;
typedef wxAuiToolBar DocumentBar;

class UndoManager;
class PluginTool;

class Document
{
public:

	Document();
	virtual ~Document();

	// Creates and initializes the document.
	void create();

	// Document management callbacks.
	virtual bool open() = 0;
	virtual bool save() = 0;
	virtual bool reset() = 0;

	// Document selection callbacks.
	virtual void onDocumentSelect() {}
	virtual void onDocumentUnselect() {}

	// Undo state management.
	void createUndo();
	void resetUndo();

	// Gets the path to the document.
	GETTER(Path, const Path&, path)

	// Sets the path to the document.
	void setPath(const Path&);

	// Gets the undo manager of the document.
	GETTER(UndoManager, UndoManager*, undoManager)

	// Gets if the document has unsaved changes.
	GETTER(UnsavedChanges, bool, unsavedChanges)

	// Gets the document notebook window.
	GETTER(Window, DocumentWindow*, documentWindow)

	// Creates the document window.
	virtual DocumentWindow* createDocumentWindow() = 0;

	// Creates a context toolbar.
	virtual DocumentBar* createDocumentBar();

	// Tool selection callbacks.
	virtual void onToolSelect(PluginTool* tool) {}

	// Handles undo/redo events.
	void onUndoRedoEvent();

protected:

	// Path to the document. 
	Path path;

	// Keeps if the document contains unsaved changes.
	bool unsavedChanges;

	// Window representing the document.
	DocumentWindow* documentWindow;

	UndoManager* undoManager;
};

//-----------------------------------//

NAMESPACE_EDITOR_END