/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Editor/API.h"
#include "LogPlugin.h"
#include "LogFrame.h"

#include "Editor.h"
#include "EditorIcons.h"

namespace vapor { namespace editor {

//-----------------------------------//

REFLECT_CHILD_CLASS(LogPlugin, EditorPlugin)
REFLECT_CLASS_END()

//-----------------------------------//

LogPlugin::LogPlugin()
{ }

//-----------------------------------//

PluginMetadata LogPlugin::getMetadata()
{
	PluginMetadata metadata;
	
	metadata.name = "Log";
	metadata.description = "Provides a view of the logs.";
	metadata.author = "triton";
	metadata.version = "1.0";

	return metadata;
}

//-----------------------------------//

void LogPlugin::onPluginEnable()
{
	wxAuiToolBar* toolbarCtrl = editor->getToolbar();
	
	if(toolbarCtrl)
	{
		addTool( toolbarCtrl->AddSeparator() );

		wxBitmap iconLog = wxMEMORY_BITMAP(page_white_text);
		logButton = toolbarCtrl->AddTool( wxID_ANY, "Log", iconLog );
		addTool( logButton );

		toolbarCtrl->Bind( wxEVT_COMMAND_TOOL_CLICKED,
			&LogPlugin::onLogButtonClick, this, logButton->GetId() );
	}

	log = new LogFrame(editor);
	log->SetSize(500, 300);
	
	wxBitmap icon = wxMEMORY_BITMAP(page_white_text);
	const char* name = "Log";

	wxAuiPaneInfo pane;
	pane.Name(name).Caption(name).Bottom().Float().Hide().Icon(icon);

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