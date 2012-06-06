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

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(LogPlugin, EditorPlugin)
REFLECT_CLASS_END()

//-----------------------------------//

LogPlugin::LogPlugin()
{
}

//-----------------------------------//

PluginMetadata LogPlugin::getMetadata()
{
	static PluginMetadata metadata;
	
	metadata.name = "Log";
	metadata.description = "Provides a view of the logs.";
	metadata.author = "triton";
	metadata.version = "1.0";
	metadata.priority = 3;

	return metadata;
}

//-----------------------------------//

void LogPlugin::onPluginEnable()
{
	wxAuiToolBar* toolbarCtrl = editor->getToolbar();

#ifdef ENABLE_LOG_TOOL_BUTTON
	if(toolbarCtrl)
	{
		addTool( toolbarCtrl->AddSeparator() );

		wxBitmap iconLog = wxMEMORY_BITMAP(page_white_text);
		logButton = toolbarCtrl->AddTool( wxID_ANY, "Log", iconLog );
		addTool( logButton );

		toolbarCtrl->Bind( wxEVT_COMMAND_TOOL_CLICKED,
			&LogPlugin::onLogButtonClick, this, logButton->GetId() );
	}
#endif

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
{
#ifdef ENABLE_LOG_TOOL_BUTTON
	wxAuiToolBar* toolbarCtrl = editor->getToolbar();
	
	if(toolbarCtrl)
	{
		toolbarCtrl->Unbind( wxEVT_COMMAND_TOOL_CLICKED,
			&LogPlugin::onLogButtonClick, this, logButton->GetId() );
	}
#endif

	editor->getAUI()->DetachPane(log);
}

//-----------------------------------//

void LogPlugin::onLogButtonClick(wxCommandEvent& event)
{
	wxAuiPaneInfo& pane = editor->getAUI()->GetPane(log);
	if( !pane.IsOk() ) return;

	pane.Show( !pane.IsShown() );

	editor->getAUI()->Update();
}

//-----------------------------------//

NAMESPACE_EDITOR_END