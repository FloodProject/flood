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

class UndoManager;

//-----------------------------------//

class UndoPlugin : public Plugin
{
public:

	UndoPlugin( EditorFrame* frame );

	// Gets metadata about this plugin.
	virtual PluginMetadata getMetadata();

	// Plugin enable callback.
	virtual void onPluginEnable( wxToolBar* );

	// Plugin disable callback.
	virtual void onPluginDisable( wxToolBar* );

protected:

	// Handles undo button click.
	void onUndoButtonClick(wxCommandEvent& event);

	// Handles redo button click.
	void onRedoButtonClick(wxCommandEvent& event);

	// Manages undo/redo operations.
	UndoManager* undoManager;

	// Undo toolbar button.
	wxToolBarToolBase* undoButton;

	// Redo toolbar button.
	wxToolBarToolBase* redoButton;
};

//-----------------------------------//

} } // end namespaces