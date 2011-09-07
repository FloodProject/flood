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
#include "EventManager.h"
#include "UndoManager.h"
#include "EditorIcons.h"
#include "Plugins/Scene/SceneDocument.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(ProjectPlugin, EditorPlugin)
REFLECT_CLASS_END()

REFLECT_CHILD_CLASS(Project, Object)
REFLECT_CLASS_END()

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

bool ProjectPlugin::askSaveChanges( Document* document )
{
	if( !document ) return false;
	if( !document->getUnsavedChanges() ) return true;

	const char* msg = "Document contains unsaved changes. Do you want to save them?";
	int flags = wxYES_NO | wxCANCEL | wxICON_EXCLAMATION;
	
	wxMessageDialog dialog(editor, msg, "Editor", flags);
	//dialog.SetSetYesNoLabels(wxID_SAVE, "&Don't save");

    int answer = dialog.ShowModal();
	if( answer == wxID_YES ) return false;

	return (answer != wxID_CANCEL);
}

//-----------------------------------//

Document* ProjectPlugin::createDocument()
{
	SceneDocument* document = AllocateThis(SceneDocument);
	GetEditor().getDocumentManager()->addDocument(document);
	return document;
}

//-----------------------------------//

void ProjectPlugin::onNewButtonClick(wxCommandEvent& event)
{
	Document* document = editor->getDocument();

	if( !document )
	{
		document = createDocument();
		return;
	}

	if( !askSaveChanges(document) )
		return;

	if( !document->reset() )
	{
		const char* msg = "Sorry, creating new documents is disabled";
		wxMessageBox(msg, wxMessageBoxCaptionStr, wxICON_ERROR | wxOK);
		return;
	}

	// Simulate destroy and create events.
	EventManager* events = GetEditor().getEventManager();
	events->onDocumentUnselect(*document);
	events->onDocumentDestroy(*document);
	events->onDocumentCreate(*document);
	events->onDocumentSelect(*document);
}

//-----------------------------------//


void ProjectPlugin::onOpenButtonClick(wxCommandEvent& event)
{
	Document* document = editor->getDocument();
	if( !document ) return;

	if( !askSaveChanges(document) )
		return;

	if( !document->open() )
	{
		const char* msg = "Could not load document.";
		wxMessageDialog message(&GetEditor(), msg, "Load", wxOK | wxICON_EXCLAMATION);
		message.ShowModal();
	}
}

//-----------------------------------//

void ProjectPlugin::onSaveButtonClick(wxCommandEvent& event)
{
	Document* document = editor->getDocument();
	if( !document ) return;

	if( !document->save() )
	{
		const char* msg = "Could not save document.";
		wxMessageDialog message(&GetEditor(), msg, "Save", wxOK | wxICON_EXCLAMATION);
		message.ShowModal();
	}
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

NAMESPACE_EDITOR_END