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

void UndoPlugin::onPluginEnable(wxToolBar* toolBar)
{
	toolBar->AddSeparator();

	wxBitmap iconUndo = wxMEMORY_BITMAP(arrow_undo);
	undoButton = toolBar->AddTool( wxID_ANY, "Undo", iconUndo );
	
	toolBar->Bind( wxEVT_COMMAND_TOOL_CLICKED,
		&UndoPlugin::onUndoButtonClick, this, undoButton->GetId() );

	wxBitmap iconRedo = wxMEMORY_BITMAP(arrow_redo);
	redoButton = toolBar->AddTool( wxID_ANY, "Redo", iconRedo );

	toolBar->Bind( wxEVT_COMMAND_TOOL_CLICKED,
		&UndoPlugin::onRedoButtonClick, this, redoButton->GetId() );
}

//-----------------------------------//

void UndoPlugin::onPluginDisable(wxToolBar* toolBar)
{
	int id;
	
	id = undoButton->GetId();
	toolBar->DeleteTool(id);

	id = redoButton->GetId();
	toolBar->DeleteTool(id);
}

//-----------------------------------//

void UndoPlugin::onUndoButtonClick(wxCommandEvent& event)
{
	bool undoEmpty = undoManager->undoOperation();
	//toolBar->EnableTool( undoButton->GetId(), !undoEmpty );
}

//-----------------------------------//

void UndoPlugin::onRedoButtonClick(wxCommandEvent& event)
{
	bool redoEmpty = undoManager->redoOperation();
	//toolBar->EnableTool( redoButton->GetId(), !redoEmpty );
}

//-----------------------------------//

} } // end namespaces