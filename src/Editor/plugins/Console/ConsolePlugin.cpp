/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "PCH.h"
#include "ConsolePlugin.h"
#include "ConsoleTextCtrl.h"

#include "Editor.h"
#include "EditorIcons.h"

namespace vapor { namespace editor {

//-----------------------------------//

ConsolePlugin::ConsolePlugin( EditorFrame* frame )
	: Plugin(frame)
{ }

//-----------------------------------//

PluginMetadata ConsolePlugin::getMetadata()
{
	PluginMetadata metadata;
	
	metadata.name = "Console";
	metadata.description = "Provides an interactive scripting console";
	metadata.author = "triton";
	metadata.version = "1.0";

	return metadata;
}

//-----------------------------------//

void ConsolePlugin::onPluginEnable()
{
	wxAuiToolBar* toolbarCtrl = editor->getToolbar();
	wxBitmap iconConsole = wxMEMORY_BITMAP(application_xp_terminal);

	if(toolbarCtrl)
	{
		addTool( toolbarCtrl->AddSeparator() );

		consoleButton = toolbarCtrl->AddTool( wxID_ANY, "Console", iconConsole );
		addTool( consoleButton );

		toolbarCtrl->Bind( wxEVT_COMMAND_TOOL_CLICKED,
			&ConsolePlugin::onConsoleButtonClick, this, consoleButton->GetId() );
	}

	console = new ConsoleTextCtrl(editor);
	console->SetSize(400, 200);

	wxAuiPaneInfo pane;
	pane.Caption("Console").Bottom().Dock().MaximizeButton()/*.Float()*/.Hide().Icon(iconConsole).Resizable();
	
	editor->getAUI()->AddPane(console, pane);
	editor->getAUI()->Update();
}

//-----------------------------------//

void ConsolePlugin::onPluginDisable()
{
	editor->getAUI()->DetachPane(console);
	editor->getAUI()->Update();

	delete console;
	console = nullptr;
}

//-----------------------------------//

void ConsolePlugin::onConsoleButtonClick(wxCommandEvent& event)
{
	wxAuiPaneInfo& pane = editor->getAUI()->GetPane(console);
	pane.Show( !pane.IsShown() );

	editor->getAUI()->Update();
}

//-----------------------------------//

} } // end namespaces