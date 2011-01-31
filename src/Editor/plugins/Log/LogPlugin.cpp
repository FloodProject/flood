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
	wxAuiToolBar* toolBar = editor->getToolbar();
	
	if(toolBar)
	{
		addTool( toolBar->AddSeparator() );

		wxBitmap iconLog = wxMEMORY_BITMAP(page_white_text);
		logButton = toolBar->AddTool( wxID_ANY, "Log", iconLog );
		addTool( logButton );

		toolBar->Bind( wxEVT_COMMAND_TOOL_CLICKED,
			&LogPlugin::onLogButtonClick, this, logButton->GetId() );
	}

	log = new LogFrame(editor);
	log->SetSize(500, 300);
	
	wxBitmap icon = wxMEMORY_BITMAP(page_white_text);

	wxAuiPaneInfo pane;
	pane.Caption("Log").Bottom().Dock().Hide().Icon(icon);

	editor->getAUI()->AddPane(log, pane);
	editor->getAUI()->Update();
}

//-----------------------------------//

void LogPlugin::onPluginDisable()
{ }

//-----------------------------------//

void LogPlugin::onLogButtonClick(wxCommandEvent& event)
{
	wxAuiPaneInfo& pane = editor->getAUI()->GetPane(log);

	if( !pane.IsOk() )
		return;

	pane.Show( !pane.IsShown() );

	editor->getAUI()->Update();
}

//-----------------------------------//

} } // end namespaces