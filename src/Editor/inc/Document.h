/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

class wxWindow;

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

class UndoManager;
class PluginTool;

class Document
{
public:

	Document();
	virtual ~Document();

	// Document management callbacks.
	virtual bool open() = 0;
	virtual bool save() = 0;
	virtual bool reset() = 0;

	// Document selection callbacks.
	virtual void onDocumentSelect() {}
	virtual void onDocumentUnselect() {}

	// Undo management.
	void createUndo();
	void resetUndo();

	// Gets the name of the document.
	GETTER(Name, const String&, name)

	// Gets the undo manager of the document.
	GETTER(UndoManager, UndoManager*, undoManager)

	// Gets if the document has unsaved changes.
	GETTER(UnsavedChanges, bool, unsavedChanges)

	// Gets the document notebook window.
	virtual wxWindow* getWindow() = 0;

	// Creates a context toolbar.
	virtual wxAuiToolBar* createContextToolbar() = 0;

	// Tool selection callbacks.
	virtual void onToolSelect(PluginTool* tool) {}

	// Handles undo/redo events.
	void onUndoRedoEvent();

	String name;
	bool unsavedChanges;
	UndoManager* undoManager;
};

//-----------------------------------//

NAMESPACE_EDITOR_END