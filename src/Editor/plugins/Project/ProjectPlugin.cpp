/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Editor/API.h"
#include "ProjectPlugin.h"
#include "Editor.h"
#include "Events.h"
#include "UndoManager.h"
#include "EditorIcons.h"
#include "../Scene/SceneDocument.h"
#include "Core/Serialization.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(ProjectPlugin, EditorPlugin)
REFLECT_CLASS_END()

REFLECT_CHILD_CLASS(Project, Object)
REFLECT_CLASS_END()

//-----------------------------------//

ProjectPlugin::ProjectPlugin()
{
}

//-----------------------------------//

PluginMetadata ProjectPlugin::getMetadata()
{
	PluginMetadata metadata;
	
	metadata.name = "Project";
	metadata.description = "Provides project management functionality.";
	metadata.author = "triton";
	metadata.version = "1.0";
	metadata.priority = 10;

	return metadata;
}

//-----------------------------------//

void ProjectPlugin::onPluginEnable()
{
	wxAuiToolBar* toolbarCtrl = editor->getToolbar();

	if(toolbarCtrl)
	{
		wxBitmap iconNew = wxMEMORY_BITMAP(page_empty);
		newButton = toolbarCtrl->AddTool( wxID_ANY, "New", iconNew, "Creates a new scene" );
		addTool( newButton );

		wxBitmap iconOpen = wxMEMORY_BITMAP(folder_explore);
		openButton = toolbarCtrl->AddTool( wxID_ANY, "Open", iconOpen, "Opens an existing scene" );
		addTool( openButton );

		wxBitmap iconSave = wxMEMORY_BITMAP(disk);
		saveButton = toolbarCtrl->AddTool( wxID_ANY, "Save", iconSave, "Save the current scene" );
		addTool( saveButton );
		
		toolbarCtrl->Bind( wxEVT_COMMAND_TOOL_CLICKED, &ProjectPlugin::onNewButtonClick, this, newButton->GetId() );
		toolbarCtrl->Bind( wxEVT_COMMAND_TOOL_CLICKED, &ProjectPlugin::onOpenButtonClick, this, openButton->GetId() );
		toolbarCtrl->Bind( wxEVT_COMMAND_TOOL_CLICKED, &ProjectPlugin::onSaveButtonClick, this, saveButton->GetId() );
		toolbarCtrl->Bind( wxEVT_UPDATE_UI, &ProjectPlugin::onSaveButtonUpdateUI, this, saveButton->GetId() );
	}

	wxMenu* menu = editor->menuFile;
	newItem = menu->Append(newButton->GetId(), newButton->GetLabel());
	openItem = menu->Append(openButton->GetId(), openButton->GetLabel());
	saveItem = menu->Append(saveButton->GetId(), saveButton->GetLabel());

	editor->Bind( wxEVT_COMMAND_TOOL_CLICKED, &ProjectPlugin::onNewButtonClick, this, newButton->GetId() );
	editor->Bind( wxEVT_COMMAND_TOOL_CLICKED, &ProjectPlugin::onOpenButtonClick, this, openButton->GetId() );
	editor->Bind( wxEVT_COMMAND_TOOL_CLICKED, &ProjectPlugin::onSaveButtonClick, this, saveButton->GetId() );
}

//-----------------------------------//

void ProjectPlugin::onPluginDisable()
{
}

//-----------------------------------//

void ProjectPlugin::onNewButtonClick(wxCommandEvent& event)
{
	Document* current = editor->getDocument();
	
#if 0
	if(current)
	{
		wxMessageBox("Sorry, creating new documents is disabled",
			wxMessageBoxCaptionStr, wxICON_ERROR | wxOK);
		return;
	}
#endif

	if( current && !askSaveChanges() )
		return;

	SceneDocument* document = new SceneDocument();
	editor->addDocument(document);

	switchScene(document);

	Events* events = GetEditor().getEventManager();
	events->onDocumentCreate(*document);
}

//-----------------------------------//

static const std::string fileDialogDescription( "Scene files (*.scene)|*.scene" );

void ProjectPlugin::onOpenButtonClick(wxCommandEvent& event)
{
	if( !askSaveChanges() )
		return;

	// Ask for file name to open.
	wxFileDialog fc( editor, wxFileSelectorPromptStr, wxEmptyString,
		wxEmptyString, fileDialogDescription, wxFC_OPEN );
	
	if( fc.ShowModal() != wxID_OK )
		return;

	Path path = (String) fc.GetPath();
	Scene* object = (Scene*) SerializerLoadObjectFromFile(AllocatorGetThis(), path);

	if( !object )
	{
		wxMessageDialog message(editor, "Could not load scene.", "Load", wxOK | wxICON_EXCLAMATION);
		message.ShowModal();
		return;
	}

	SceneDocument* document = (SceneDocument*) editor->getDocument();
	document->scene = ScenePtr(object);

	switchScene(document);
}

//-----------------------------------//

bool ProjectPlugin::askSaveChanges()
{
	Document* document = editor->getDocument();
	if( !document ) return false;

	if( !document->getUnsavedChanges() )
		return true;

	wxMessageDialog dialog(editor,  
		"Scene contains unsaved changes. Do you want to save them?",
		"Editor", wxYES_NO | wxCANCEL | wxICON_EXCLAMATION);

	//dialog.SetSetYesNoLabels(wxID_SAVE, "&Don't save");

    int answer = dialog.ShowModal();

     if( answer == wxID_YES && !saveScene() )
		return false;

	return (answer != wxID_CANCEL);
}

//-----------------------------------//

bool ProjectPlugin::saveScene()
{
	// Ask for file name to save as.
	wxFileDialog fc( editor, wxFileSelectorPromptStr, wxEmptyString,
		wxEmptyString, fileDialogDescription, wxFC_SAVE | wxFD_OVERWRITE_PROMPT );
	
	if( fc.ShowModal() != wxID_OK )
		return false;

	Path path = (String) fc.GetPath();
	
	SceneDocument* sceneDocument = (SceneDocument*) GetEditor().getDocument();	
	ScenePtr scene = sceneDocument->scene;

	return SerializerSaveObjectToFile(AllocatorGetThis(), path, scene.get());
}

//-----------------------------------//

void ProjectPlugin::onSaveButtonClick(wxCommandEvent& event)
{
	saveScene();
}

//-----------------------------------//

void ProjectPlugin::onSaveButtonUpdateUI(wxUpdateUIEvent& event)
{
	Document* document = editor->getDocument();
	
	if( !document )
	{
		event.Enable(false);
		return;
	}

	event.Enable( document->getUnsavedChanges() );
}

//-----------------------------------//

void ProjectPlugin::switchScene(SceneDocument* document)
{
	if( !document ) return;
	
	UndoManager* undo = document->getUndoManager();
	undo->clearOperations();
	document->unsavedChanges = false;

	const ScenePtr& scene = document->scene;
	GetEngine()->setScene(scene);

#ifdef ENABLE_PHYSICS_BULLET
	delete engine->getPhysicsManager();
	PhysicsManager* physics = new PhysicsManager();
	physics->createWorld();
	engine->setPhysicsManager(physics);
#endif

	Events* events = editor->getEventManager();
	events->onSceneLoad(scene);

	editor->redrawView();
}

//-----------------------------------//

NAMESPACE_EDITOR_END