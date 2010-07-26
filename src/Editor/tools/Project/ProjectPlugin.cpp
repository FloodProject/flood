/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "PCH.h"
#include "ProjectPlugin.h"
#include "Editor.h"
#include "EditorIcons.h"

namespace vapor { namespace editor {

//-----------------------------------//

ProjectPlugin::ProjectPlugin( EditorFrame* frame )
	: Plugin(frame)
{ }

//-----------------------------------//

PluginMetadata ProjectPlugin::getMetadata()
{
	PluginMetadata metadata;
	
	metadata.name = "Project";
	metadata.description = "Provides project management functionality";
	metadata.author = "triton";
	metadata.version = "1.0";

	return metadata;
}

//-----------------------------------//

void ProjectPlugin::onPluginEnable(wxToolBar* toolBar)
{
	PluginMetadata metadata = getMetadata();

	toolBar->AddSeparator();

	wxBitmap iconNew = wxMEMORY_BITMAP(page_empty);
	newButton = toolBar->AddTool( wxID_ANY, "New", iconNew );
	
	toolBar->Bind( wxEVT_COMMAND_TOOL_CLICKED,
		&ProjectPlugin::onNewButtonClick, this, newButton->GetId() );

	wxBitmap iconOpen = wxMEMORY_BITMAP(folder_explore);
	openButton = toolBar->AddTool( wxID_ANY, "Open", iconOpen );

	toolBar->Bind( wxEVT_COMMAND_TOOL_CLICKED,
		&ProjectPlugin::onOpenButtonClick, this, openButton->GetId() );

	wxBitmap iconSave = wxMEMORY_BITMAP(disk);
	saveButton = toolBar->AddTool( wxID_ANY, "Save", iconSave );

	toolBar->Bind( wxEVT_COMMAND_TOOL_CLICKED,
		&ProjectPlugin::onSaveButtonClick, this, saveButton->GetId() );
}

//-----------------------------------//

void ProjectPlugin::onPluginDisable(wxToolBar* toolBar)
{
	int id;
	
	id = newButton->GetId();
	toolBar->DeleteTool(id);

	id = openButton->GetId();
	toolBar->DeleteTool(id);

	id = saveButton->GetId();
	toolBar->DeleteTool(id);
}

//-----------------------------------//

void ProjectPlugin::onNewButtonClick(wxCommandEvent& event)
{

}

//-----------------------------------//

void ProjectPlugin::onOpenButtonClick(wxCommandEvent& event)
{

}

//-----------------------------------//

void ProjectPlugin::onSaveButtonClick(wxCommandEvent& event)
{
	// Ask for file name to save as.
	wxFileDialog fc( editor, wxFileSelectorPromptStr, wxEmptyString,
		wxEmptyString, "Scene files (*.scene)|*.scene", wxFC_SAVE );
	
	if( fc.ShowModal() != wxID_OK )
		return;

	Engine* engine = editor->getEngine();
	ScenePtr scene = engine->getSceneManager();
	
	// Serialize scene to JSON.
	Json::Value sceneJSON;
	scene->serialize( sceneJSON );

	// Save it to a file.
	std::string fn( fc.GetPath() );
	serializeToFile( sceneJSON, fn );
}

//-----------------------------------//

} } // end namespaces