/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Editor/API.h"
#include "UndoPlugin.h"
#include "UndoManager.h"
#include "Editor.h"
#include "EditorIcons.h"
#include "Events.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(UndoPlugin, EditorPlugin)
REFLECT_CLASS_END()

//-----------------------------------//

UndoPlugin::UndoPlugin()
	: undoManager(nullptr)
{ }

//-----------------------------------//

PluginMetadata UndoPlugin::getMetadata()
{
	PluginMetadata metadata;
	
	metadata.name = "Undo/Redo";
	metadata.description = "Provides undo and redo functionality.";
	metadata.author = "triton";
	metadata.version = "1.0";
	metadata.priority = 12;

	return metadata;
}

//-----------------------------------//

void UndoPlugin::onPluginEnable()
{
	wxAuiToolBar* toolbarCtrl = editor->getToolbar();
	
	if(toolbarCtrl)
	{
		addTool( toolbarCtrl->AddSeparator() );

		wxBitmap iconUndo = wxMEMORY_BITMAP(arrow_undo);
		undoButton = toolbarCtrl->AddTool( wxID_UNDO, "Undo", iconUndo, "Undo" );
		addTool(undoButton);

		wxBitmap iconRedo = wxMEMORY_BITMAP(arrow_redo);
		redoButton = toolbarCtrl->AddTool( wxID_REDO, "Redo", iconRedo, "Redo" );
		addTool(redoButton);

		toolbarCtrl->Bind( wxEVT_COMMAND_TOOL_CLICKED, &UndoPlugin::onUndoButtonClick, this, undoButton->GetId() );
		toolbarCtrl->Bind( wxEVT_COMMAND_TOOL_CLICKED, &UndoPlugin::onRedoButtonClick, this, redoButton->GetId() );
	}

	wxMenu* menu = editor->menuEdit;
	undoItem = menu->Append(undoButton->GetId(), undoButton->GetLabel() + "\tCtrl-Z");
	redoItem = menu->Append(redoButton->GetId(), redoButton->GetLabel() + "\tCtrl-Y");

	editor->Bind( wxEVT_COMMAND_TOOL_CLICKED, &UndoPlugin::onUndoButtonClick, this, undoButton->GetId() );
	editor->Bind( wxEVT_COMMAND_TOOL_CLICKED, &UndoPlugin::onRedoButtonClick, this, redoButton->GetId() );

	// Subscribe as an event listener.
	Events* events = editor->getEventManager();
	events->addEventListener(this);

	updateButtons();
}

//-----------------------------------//

void UndoPlugin::onPluginDisable()
{
	// Unsubscribe as an event listener.
	Events* events = editor->getEventManager();
	events->removeEventListener(this);
}

//-----------------------------------//

void UndoPlugin::onDocumentSelect( Document& document )
{
	// Connect to undo/redo events.
	undoManager = document.getUndoManager();
	undoManager->onUndoRedoEvent.Connect(this, &UndoPlugin::onUndoEvent);

	// Updates the state of the buttons.
	updateButtons();
}

//-----------------------------------//

void UndoPlugin::onDocumentUnselect( Document& document )
{
	// Disconnect to undo/redo events.
	undoManager->onUndoRedoEvent.Disconnect(this, &UndoPlugin::onUndoEvent);
	undoManager = nullptr;

	// Updates the state of the buttons.
	updateButtons();
}

//-----------------------------------//

void UndoPlugin::onUndoEvent()
{
	updateButtons();
}

//-----------------------------------//

void UndoPlugin::updateButtons()
{
	bool undoEmpty = true;
	bool redoEmpty = true;

	if(undoManager)
	{
		undoEmpty = undoManager->getUndoOperations().empty();
		redoEmpty = undoManager->getRedoOperations().empty();		
	}

	undoItem->Enable( !undoEmpty );
	redoItem->Enable( !redoEmpty );

	wxAuiToolBar* toolbarCtrl = editor->getToolbar();

	if(toolbarCtrl)
	{
		toolbarCtrl->EnableTool( undoButton->GetId(), !undoEmpty );
		toolbarCtrl->EnableTool( redoButton->GetId(), !redoEmpty );
		toolbarCtrl->Refresh();
	}
}

//-----------------------------------//

void UndoPlugin::onUndoButtonClick(wxCommandEvent& event)
{
	undoManager->undoOperation();
}

//-----------------------------------//

void UndoPlugin::onRedoButtonClick(wxCommandEvent& event)
{
	undoManager->redoOperation();
}

//-----------------------------------//

NAMESPACE_EDITOR_END