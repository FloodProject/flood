/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "PCH.h"
#include "ConsolePlugin.h"
#include "ConsoleFrame.h"

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
	wxToolBar* toolBar = editor->getToolbar();

	addTool( toolBar->AddSeparator() );

	wxBitmap iconConsole = wxMEMORY_BITMAP(application_xp_terminal);
	consoleButton = toolBar->AddTool( wxID_ANY, "Console", iconConsole );
	addTool( consoleButton );

	toolBar->Bind( wxEVT_COMMAND_TOOL_CLICKED,
		&ConsolePlugin::onConsoleButtonClick, this, consoleButton->GetId() );

	console = new ConsoleFrame( editor->getEngine(), editor );
}

//-----------------------------------//

void ConsolePlugin::onPluginDisable()
{ }

//-----------------------------------//

void ConsolePlugin::onConsoleButtonClick(wxCommandEvent& event)
{
	console->Show( !console->IsShown() );
}

//-----------------------------------//

} } // end namespaces