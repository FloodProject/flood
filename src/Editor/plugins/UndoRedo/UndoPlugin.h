/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Plugin.h"

namespace vapor { namespace editor {

//-----------------------------------//

class UndoManager;

REFLECT_DECLARE_CLASS(UndoPlugin)

class UndoPlugin : public Plugin
{
	REFLECT_DECLARE_OBJECT(UndoPlugin)

public:

	UndoPlugin();

	// Gets metadata about this plugin.
	virtual PluginMetadata getMetadata();

	// Plugin callbacks.
	virtual void onPluginEnable();
	virtual void onPluginDisable();
	virtual void onDocumentSelect( Document& document );
	virtual void onDocumentUnselect( Document& document );

protected:

	// Update button states.
	void updateButtons();

	// Handles undo events.
	void onUndoEvent();

	// Handles button clicks.
	void onUndoButtonClick(wxCommandEvent& event);
	void onRedoButtonClick(wxCommandEvent& event);

	wxAuiToolBarItem* undoButton;
	wxAuiToolBarItem* redoButton;

	wxMenuItem* undoItem;
	wxMenuItem* redoItem;

	UndoManager* undoManager;
};

//-----------------------------------//

} } // end namespaces