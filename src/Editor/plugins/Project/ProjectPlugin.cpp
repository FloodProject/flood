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

#include "io/JsonSerializer.h"

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
	wxAuiToolBar* toolBar = editor->getToolbar();

	if(toolBar)
	{
		wxBitmap iconNew = wxMEMORY_BITMAP(page_empty);
		newButton = toolBar->AddTool( wxID_ANY, "New", iconNew, "Creates a new scene" );
		addTool( newButton );

		wxBitmap iconOpen = wxMEMORY_BITMAP(folder_explore);
		openButton = toolBar->AddTool( wxID_ANY, "Open", iconOpen, "Opens an existing scene" );
		addTool( openButton );

		wxBitmap iconSave = wxMEMORY_BITMAP(disk);
		saveButton = toolBar->AddTool( wxID_ANY, "Save", iconSave, "Save the current scene" );
		addTool( saveButton );
		
		toolBar->Bind( wxEVT_COMMAND_TOOL_CLICKED, &ProjectPlugin::onNewButtonClick, this, newButton->GetId() );
		toolBar->Bind( wxEVT_COMMAND_TOOL_CLICKED, &ProjectPlugin::onOpenButtonClick, this, openButton->GetId() );
		toolBar->Bind( wxEVT_COMMAND_TOOL_CLICKED, &ProjectPlugin::onSaveButtonClick, this, saveButton->GetId() );
		toolBar->Bind( wxEVT_UPDATE_UI, &ProjectPlugin::onSaveButtonUpdateUI, this, saveButton->GetId() );
	}

	wxMenu* menu = editor->fileMenu;
	newItem = menu->Append(newButton->GetId(), newButton->GetLabel());
	openItem = menu->Append(openButton->GetId(), openButton->GetLabel());
	saveItem = menu->Append(saveButton->GetId(), saveButton->GetLabel());

	editor->Bind( wxEVT_COMMAND_TOOL_CLICKED, &ProjectPlugin::onNewButtonClick, this, newButton->GetId() );
	editor->Bind( wxEVT_COMMAND_TOOL_CLICKED, &ProjectPlugin::onOpenButtonClick, this, openButton->GetId() );
	editor->Bind( wxEVT_COMMAND_TOOL_CLICKED, &ProjectPlugin::onSaveButtonClick, this, saveButton->GetId() );

	UndoManager* undo = editor->getUndoManager();
	undo->onUndoRedoEvent.Connect(this, &ProjectPlugin::onUndoRedoEvent);
}

//-----------------------------------//

void ProjectPlugin::onPluginDisable()
{
	UndoManager* undo = editor->getUndoManager();
	undo->onUndoRedoEvent.Disconnect(this, &ProjectPlugin::onUndoRedoEvent);
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

	std::string path = (std::string) fc.GetPath();
	FileStream stream(path, StreamMode::Read );
	
	if( !stream.open() )
		return;

	JsonDeserializer json( stream );
	Object* object = json.deserialize();

	if( !object )
	{
		wxMessageDialog message(editor,
			"Could not load scene.", "Load", wxOK | wxICON_EXCLAMATION);
		message.ShowModal();

		return;
	}

	ScenePtr newScene( (Scene*) object );
	switchScene(newScene);
}

//-----------------------------------//

bool ProjectPlugin::saveScene()
{
	// Ask for file name to save as.
	wxFileDialog fc( editor, wxFileSelectorPromptStr, wxEmptyString,
		wxEmptyString, fileDialogDescription, wxFC_SAVE | wxFD_OVERWRITE_PROMPT );
	
	if( fc.ShowModal() != wxID_OK )
		return false;

	Engine* engine = editor->getEngine();
	ScenePtr scene = engine->getSceneManager();
	
	std::string path = (std::string) fc.GetPath();
	FileStream stream( path, StreamMode::Write );
	
	if( !stream.open() )
		return false;

	JsonSerializer json( stream );
	
	ObjectWalker walker(json);
	walker.process(scene.get());

	unsavedChanges = false;
	return true;
}

//-----------------------------------//

bool ProjectPlugin::askSaveChanges()
{
	if( !unsavedChanges )
		return true;

	wxMessageDialog dialog(editor,  
		"Scene contains unsaved changes. Do you want to save them?",
		"Editor", wxYES_NO | wxCANCEL | wxICON_EXCLAMATION);

	//dialog.SetSetYesNoLabels(wxID_SAVE, "&Don't save");

    int answer = dialog.ShowModal();

     if( answer == wxID_YES && !saveScene() )
		return false;

	if(answer != wxID_CANCEL )
		return true;
	else
		return false;
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
	//saveItem->Enable( unsavedChanges );
}

//-----------------------------------//

void ProjectPlugin::switchScene(const ScenePtr& scene)
{
	editor->switchPlayMode(false);

	UndoManager* undo = editor->getUndoManager();
	undo->clearOperations();

	unsavedChanges = false;

	Engine* engine = editor->getEngine();
	engine->setSceneManager(scene);

#ifdef VAPOR_PHYSICS_BULLET
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

} } // end namespaces