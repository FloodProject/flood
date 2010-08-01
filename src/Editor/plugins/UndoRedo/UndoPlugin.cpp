/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "PCH.h"
#include "UndoPlugin.h"
#include "Editor.h"
#include "UndoManager.h"
#include "EditorIcons.h"

namespace vapor { namespace editor {

//-----------------------------------//

UndoPlugin::UndoPlugin( EditorFrame* frame )
	: Plugin(frame)
{
	undoManager = editor->getUndoManager();
	assert( undoManager != nullptr );
}

//-----------------------------------//

PluginMetadata UndoPlugin::getMetadata()
{
	PluginMetadata metadata;
	
	metadata.name = "Undo/Redo";
	metadata.description = "Provides undo and redo functionality";
	metadata.author = "triton";
	metadata.version = "1.0";

	return metadata;
}

//-----------------------------------//

void UndoPlugin::onPluginEnable()
{
	wxToolBar* toolBar = editor->getToolbar();
	addTool( toolBar->AddSeparator() );

	wxBitmap iconUndo = wxMEMORY_BITMAP(arrow_undo);
	undoButton = toolBar->AddTool( wxID_ANY, "Undo", iconUndo );
	addTool(undoButton);
	toolBar->EnableTool( undoButton->GetId(), false );

	wxBitmap iconRedo = wxMEMORY_BITMAP(arrow_redo);
	redoButton = toolBar->AddTool( wxID_ANY, "Redo", iconRedo );
	addTool(redoButton);
	toolBar->EnableTool( redoButton->GetId(), false );

	// Connect to click events.
	toolBar->Bind( wxEVT_COMMAND_TOOL_CLICKED,
		&UndoPlugin::onUndoButtonClick, this, undoButton->GetId() );

	toolBar->Bind( wxEVT_COMMAND_TOOL_CLICKED,
		&UndoPlugin::onRedoButtonClick, this, redoButton->GetId() );

	// Connect to undo/redo events.
	undoManager->onUndoRedoEvent +=
		fd::bind(&UndoPlugin::onUndoEvent, this);
}

//-----------------------------------//

void UndoPlugin::onPluginDisable()
{
	// Disconnect to undo/redo events.
	undoManager->onUndoRedoEvent -=
		fd::bind(&UndoPlugin::onUndoEvent, this);
}

//-----------------------------------//

void UndoPlugin::onUndoEvent()
{
	updateButtons();
	editor->RefreshViewport();
}

//-----------------------------------//

void UndoPlugin::updateButtons()
{
	wxToolBar* toolBar = editor->getToolbar();

	const Operations& undoOperations = undoManager->getUndoOperations();
	bool undoEmpty = undoOperations.empty();

	const Operations& redoOperations = undoManager->getRedoOperations();
	bool redoEmpty = redoOperations.empty();

	toolBar->EnableTool( undoButton->GetId(), !undoEmpty );
	toolBar->EnableTool( redoButton->GetId(), !redoEmpty );
}

//-----------------------------------//

void UndoPlugin::onUndoButtonClick(wxCommandEvent& event)
{
	undoManager->undoOperation();
	updateButtons();
}

//-----------------------------------//

void UndoPlugin::onRedoButtonClick(wxCommandEvent& event)
{
	undoManager->redoOperation();
	updateButtons();
}

//-----------------------------------//

} } // end namespaces