/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "PCH.h"
#include "LogPlugin.h"
#include "LogFrame.h"

#include "Editor.h"
#include "EditorIcons.h"

namespace vapor { namespace editor {

//-----------------------------------//

LogPlugin::LogPlugin( EditorFrame* frame )
	: Plugin(frame)
{ }

//-----------------------------------//

PluginMetadata LogPlugin::getMetadata()
{
	PluginMetadata metadata;
	
	metadata.name = "Log";
	metadata.description = "Shows the main log stream";
	metadata.author = "triton";
	metadata.version = "1.0";

	return metadata;
}

//-----------------------------------//

void LogPlugin::onPluginEnable()
{
	wxToolBar* toolBar = editor->getToolbar();

	addTool( toolBar->AddSeparator() );

	wxBitmap iconLog = wxMEMORY_BITMAP(page_white_text);
	logButton = toolBar->AddTool( wxID_ANY, "Log", iconLog );
	addTool( logButton );

	toolBar->Bind( wxEVT_COMMAND_TOOL_CLICKED,
		&LogPlugin::onLogButtonClick, this, logButton->GetId() );

	log = new LogFrame( editor->getEngine(), editor );
	log->SetSize(300, 200);
}

//-----------------------------------//

void LogPlugin::onPluginDisable()
{ }

//-----------------------------------//

void LogPlugin::onLogButtonClick(wxCommandEvent& event)
{
	log->Show( !log->IsShown() );
}

//-----------------------------------//

} } // end namespaces