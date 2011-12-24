/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "EditorPlugin.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

class UndoManager;

REFLECT_DECLARE_CLASS(UndoPlugin)

class UndoPlugin : public EditorPlugin
{
	REFLECT_DECLARE_OBJECT(UndoPlugin)

public:

	UndoPlugin();

	// Gets metadata about this plugin.
	PluginMetadata getMetadata() OVERRIDE;

	// Plugin callbacks.
	void onPluginEnable() OVERRIDE;
	void onPluginDisable() OVERRIDE;
	
	// Document callbacks.
	void onDocumentDestroy( Document& ) OVERRIDE;
	void onDocumentSelect( Document& ) OVERRIDE;
	void onDocumentUnselect( Document& ) OVERRIDE;

	// Undo management.
	void connectUndo(Document&);
	void disconnectUndo(Document&);

protected:

	// Update button states.
	void updateButtons();

	// Handles undo events.
	void onUndoEvent();

	// Handles button clicks.
	void onUndoButton(wxCommandEvent& event);
	void onRedoButton(wxCommandEvent& event);

	wxAuiToolBarItem* undoButton;
	wxAuiToolBarItem* redoButton;

	wxMenuItem* undoItem;
	wxMenuItem* redoItem;

	Document* undoDocument;
};

//-----------------------------------//

NAMESPACE_EDITOR_END