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
#include "Events.h"
#include "UndoManager.h"
#include "EditorIcons.h"

namespace vapor { namespace editor {

//-----------------------------------//

ProjectPlugin::ProjectPlugin( EditorFrame* frame )
	: Plugin(frame)
	, unsavedChanges(false)
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

void ProjectPlugin::onPluginEnable()
{
	wxToolBar* toolBar = editor->getToolbar();

	addTool( toolBar->AddSeparator() );

	wxBitmap iconNew = wxMEMORY_BITMAP(page_empty);
	newButton = toolBar->AddTool( wxID_ANY, "New", iconNew, "Creates a new scene" );
	addTool( newButton );

	toolBar->Bind( wxEVT_COMMAND_TOOL_CLICKED,
		&ProjectPlugin::onNewButtonClick, this, newButton->GetId() );

	wxBitmap iconOpen = wxMEMORY_BITMAP(folder_explore);
	openButton = toolBar->AddTool( wxID_ANY, "Open", iconOpen, "Opens an existing scene" );
	addTool( openButton );

	toolBar->Bind( wxEVT_COMMAND_TOOL_CLICKED,
		&ProjectPlugin::onOpenButtonClick, this, openButton->GetId() );

	wxBitmap iconSave = wxMEMORY_BITMAP(disk);
	saveButton = toolBar->AddTool( wxID_ANY, "Save", iconSave, "Save the current scene" );
	addTool( saveButton );

	toolBar->Bind( wxEVT_COMMAND_TOOL_CLICKED,
		&ProjectPlugin::onSaveButtonClick, this, saveButton->GetId() );

	toolBar->Bind( wxEVT_UPDATE_UI,
		&ProjectPlugin::onSaveButtonUpdateUI, this, saveButton->GetId() );

	UndoManager* undo = editor->getUndoManager();
	undo->onUndoRedoEvent += fd::bind(&ProjectPlugin::onUndoRedoEvent, this);
}

//-----------------------------------//

void ProjectPlugin::onPluginDisable()
{
	UndoManager* undo = editor->getUndoManager();
	undo->onUndoRedoEvent -= fd::bind(&ProjectPlugin::onUndoRedoEvent, this);
}

//-----------------------------------//

void ProjectPlugin::onUndoRedoEvent()
{
	unsavedChanges = true;
}

//-----------------------------------//

void ProjectPlugin::onNewButtonClick(wxCommandEvent& event)
{
	if( !askSaveChanges() )
		return;

	ScenePtr scene( new Scene() );
	switchScene(scene);
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
	
	Serializer deserializer;
	deserializer.openFromFile( (std::string) fc.GetPath() );
	
	ScenePtr newScene = deserializer.deserializeScene();
	switchScene(newScene);
}

//-----------------------------------//

void ProjectPlugin::onSaveButtonClick(wxCommandEvent& event)
{
	saveScene();
}

//-----------------------------------//

void ProjectPlugin::onSaveButtonUpdateUI(wxUpdateUIEvent& event)
{
	event.Enable( unsavedChanges );
}

//-----------------------------------//

void ProjectPlugin::switchScene(const ScenePtr& scene)
{
	editor->switchPlayMode(false);

	UndoManager* undo = editor->getUndoManager();
	undo->clearOperations();

	unsavedChanges = false;

	Engine* engine = editor->getEngine();
	delete engine->getPhysicsManager();

	PhysicsManager* physics = new PhysicsManager();
	physics->createWorld();

	engine->setPhysicsManager(physics);
	
	engine->setSceneManager(scene);

	Events* events = editor->getEventManager();
	events->onSceneLoad(scene);

	editor->redrawView();
}

//-----------------------------------//

bool ProjectPlugin::saveScene()
{
	// Ask for file name to save as.
	wxFileDialog fc( editor, wxFileSelectorPromptStr, wxEmptyString,
		wxEmptyString, fileDialogDescription, wxFC_SAVE );
	
	if( fc.ShowModal() != wxID_OK )
		return false;

	Engine* engine = editor->getEngine();
	ScenePtr scene = engine->getSceneManager();
	
	Serializer serializer;
	serializer.serializeScene(scene);
	serializer.saveToFile( (std::string) fc.GetPath() );

	unsavedChanges = false;
	return true;
}

//-----------------------------------//

bool ProjectPlugin::askSaveChanges()
{
	if( !unsavedChanges )
		return true;

    int answer = wxMessageBox(
		"Scene contains unsaved changes. Do you want to save them?",
		wxEmptyString, wxYES_NO | wxCANCEL, editor);

    if( answer == wxYES && !saveScene() )
		return false;

	if(answer != wxCANCEL)
		return true;
	else
		return false;
}

//-----------------------------------//

} } // end namespaces