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
#include "EventManager.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(UndoPlugin, EditorPlugin)
REFLECT_CLASS_END()

//-----------------------------------//

UndoPlugin::UndoPlugin()
{

}

//-----------------------------------//

PluginMetadata UndoPlugin::getMetadata()
{
	static PluginMetadata metadata;
	
	metadata.name = "Undo/Redo";
	metadata.description = "Provides undo and redo functionality.";
	metadata.author = "triton";
	metadata.version = "1.0";
	metadata.priority = 200;

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

		toolbarCtrl->Bind( wxEVT_COMMAND_TOOL_CLICKED, &UndoPlugin::onUndoButton, this, undoButton->GetId() );
		toolbarCtrl->Bind( wxEVT_COMMAND_TOOL_CLICKED, &UndoPlugin::onRedoButton, this, redoButton->GetId() );
	}

	wxMenu* menu = editor->menuEdit;
	undoItem = menu->Append(undoButton->GetId(), undoButton->GetLabel() + "\tCtrl-Z");
	redoItem = menu->Append(redoButton->GetId(), redoButton->GetLabel() + "\tCtrl-Y");

	editor->Bind( wxEVT_COMMAND_TOOL_CLICKED, &UndoPlugin::onUndoButton, this, undoButton->GetId() );
	editor->Bind( wxEVT_COMMAND_TOOL_CLICKED, &UndoPlugin::onRedoButton, this, redoButton->GetId() );

	// Subscribe as an event listener.
	EventManager* events = editor->getEventManager();
	events->addEventListener(this);

	undoDocument = nullptr;

	updateButtons();
}

//-----------------------------------//

void UndoPlugin::onPluginDisable()
{
	if( undoDocument )
		disconnectUndo(*undoDocument);

	// Remove entries from the menu.
	wxMenu* menu = editor->menuEdit;
	menu->Delete( undoItem->GetId() );
	menu->Delete( redoItem->GetId() );

	// Disconnect from all the events.
	editor->Unbind( wxEVT_COMMAND_TOOL_CLICKED, &UndoPlugin::onUndoButton, this, undoButton->GetId() );
	editor->Unbind( wxEVT_COMMAND_TOOL_CLICKED, &UndoPlugin::onRedoButton, this, redoButton->GetId() );

	wxAuiToolBar* toolbarCtrl = editor->getToolbar();
	
	if(toolbarCtrl)
	{
		toolbarCtrl->Unbind( wxEVT_COMMAND_TOOL_CLICKED, &UndoPlugin::onUndoButton, this, undoButton->GetId() );
		toolbarCtrl->Unbind( wxEVT_COMMAND_TOOL_CLICKED, &UndoPlugin::onRedoButton, this, redoButton->GetId() );
	}
}

//-----------------------------------//

void UndoPlugin::onDocumentDestroy( Document& document )
{
	disconnectUndo(document);
}

//-----------------------------------//

void UndoPlugin::onDocumentSelect( Document& document )
{
	connectUndo(document);
}

//-----------------------------------//

void UndoPlugin::onDocumentUnselect( Document& document )
{
	disconnectUndo(document);
}

//-----------------------------------//

void UndoPlugin::connectUndo(Document& document)
{
	undoDocument = &document;
	UndoManager* undoManager = document.getUndoManager();

	// Connect to undo/redo events.
	undoManager->onUndoRedoEvent.Connect(this, &UndoPlugin::onUndoEvent);

	// Updates the state of the buttons.
	updateButtons();
}

//-----------------------------------//

void UndoPlugin::disconnectUndo(Document& document)
{
	if( undoDocument != &document ) return;

	undoDocument = nullptr;

	UndoManager* undoManager = document.getUndoManager();
	
	if( undoManager )
	{
		// Disconnect to undo/redo events.
		undoManager->onUndoRedoEvent.Disconnect(this, &UndoPlugin::onUndoEvent);
	}

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

	if(undoDocument && undoDocument->getUndoManager())
	{
		UndoManager* undoManager = undoDocument->getUndoManager();
	
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

void UndoPlugin::onUndoButton(wxCommandEvent& event)
{
	UndoManager* undoManager = undoDocument->getUndoManager();
	undoManager->undoOperation();
}

//-----------------------------------//

void UndoPlugin::onRedoButton(wxCommandEvent& event)
{
	UndoManager* undoManager = undoDocument->getUndoManager();
	undoManager->redoOperation();
}

//-----------------------------------//

NAMESPACE_EDITOR_END