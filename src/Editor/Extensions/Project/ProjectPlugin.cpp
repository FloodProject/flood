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

#if defined(PLATFORM_WINDOWS) && defined(ENABLE_MEMORY_LEAK_DETECTOR)
#include <vld.h>
#endif

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(ProjectPlugin, EditorPlugin)
REFLECT_CLASS_END()

REFLECT_CHILD_CLASS(Project, Object)
REFLECT_CLASS_END()

//-----------------------------------//

PluginMetadata ProjectPlugin::getMetadata()
{
	static PluginMetadata metadata;
	
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
		
		toolbarCtrl->Bind( wxEVT_COMMAND_TOOL_CLICKED, &ProjectPlugin::onNewDocument, this, newButton->GetId() );
		toolbarCtrl->Bind( wxEVT_COMMAND_TOOL_CLICKED, &ProjectPlugin::onOpenDocument, this, openButton->GetId() );
		toolbarCtrl->Bind( wxEVT_COMMAND_TOOL_CLICKED, &ProjectPlugin::onSaveDocument, this, saveButton->GetId() );
	}

	wxMenu* menu = editor->menuFile;
	newItem = menu->Append(newButton->GetId(), newButton->GetLabel() + "\tCtrl-N");
	openItem = menu->Append(openButton->GetId(), openButton->GetLabel() + "\tCtrl-O");
	saveItem = menu->Append(saveButton->GetId(), saveButton->GetLabel() + "\tCtrl-S");
	saveAsItem = menu->Append(wxID_ANY, "Save As...");

	editor->Bind( wxEVT_COMMAND_TOOL_CLICKED, &ProjectPlugin::onNewDocument, this, newButton->GetId() );
	editor->Bind( wxEVT_COMMAND_TOOL_CLICKED, &ProjectPlugin::onOpenDocument, this, openButton->GetId() );
	editor->Bind( wxEVT_COMMAND_TOOL_CLICKED, &ProjectPlugin::onSaveDocument, this, saveButton->GetId() );
	editor->Bind( wxEVT_COMMAND_TOOL_CLICKED, &ProjectPlugin::onSaveAsDocument, this, saveAsItem->GetId() );
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
	
#if defined(PLATFORM_WINDOWS) && defined(ENABLE_MEMORY_LEAK_DETECTOR)
	// Workaround VLD bug with file dialogs
	VLDDisable();
#endif

	wxMessageDialog dialog(editor, msg, "Editor", flags);
	int answer = dialog.ShowModal();

#if defined(PLATFORM_WINDOWS) && defined(ENABLE_MEMORY_LEAK_DETECTOR)
	VLDRestore();
#endif

	if( answer == wxID_YES ) return false;

	return (answer != wxID_CANCEL);
}

//-----------------------------------//

Document* ProjectPlugin::createDocument()
{
	SceneDocument* document = AllocateThis(SceneDocument);
	document->create();

	GetEditor().getDocumentManager()->addDocument(document);
	return document;
}

//-----------------------------------//

void ProjectPlugin::onNewDocument(wxCommandEvent& event)
{
	//Document* document = editor->getDocument();

	EventManager* events = GetEditor().getEventManager();
	Document* document = nullptr;

	if( !document )
	{
		document = createDocument();
		goto created;
	}

	if( !askSaveChanges(document) )
		return;

	// Simulate destroy and create events.
	events->onDocumentUnselect(*document);
	events->onDocumentDestroy(*document);

	if( !document->reset() )
	{
		const char* msg = "Sorry, creating new documents is disabled";
		wxMessageBox(msg, wxMessageBoxCaptionStr, wxICON_ERROR | wxOK);
		return;
	}

created:

	document->setPath("");

	events->onDocumentCreate(*document);
	events->onDocumentSelect(*document);
}

//-----------------------------------//

void ProjectPlugin::onOpenDocument(wxCommandEvent& event)
{
	Document* document = editor->getDocument();
	if( !document ) return;

	if( !askSaveChanges(document) )
		return;

	if( !document->onDocumentOpen() )
	{
		const char* msg = "Could not load document.";
		wxMessageDialog message(&GetEditor(), msg, "Load", wxOK | wxICON_EXCLAMATION);
		message.ShowModal();
	}
}

//-----------------------------------//

void ProjectPlugin::onSaveDocument(wxCommandEvent& event)
{
	Document* document = editor->getDocument();
	if( !document ) return;

	// If the document doesn't have a path, ask for it.
	if( document->getPath().empty() )
	{
		onSaveAsDocument(event);
		return;
	}

	if( !document->onDocumentSave() )
	{
		const char* msg = "Could not save document.";
		wxMessageDialog message(&GetEditor(), msg, "Save", wxOK | wxICON_EXCLAMATION);
		message.ShowModal();
	}

	document->setUnsavedChanges(false);
	GetEditor().getDocumentManager()->onDocumentRenamed(document);
}

//-----------------------------------//

void ProjectPlugin::onSaveAsDocument(wxCommandEvent& event)
{
	Document* document = editor->getDocument();
	if( !document ) return;

	int flags = wxFC_SAVE | wxFD_OVERWRITE_PROMPT;
	
	wxFileDialog fileDialog( editor, wxFileSelectorPromptStr, wxEmptyString,
		wxEmptyString, document->getFileDialogDescription(), flags );

	if( fileDialog.ShowModal() != wxID_OK )
		return;

	wxString newPath = fileDialog.GetPath();
	document->setPath((String) newPath);

	onSaveDocument(event);
}

//-----------------------------------//

NAMESPACE_EDITOR_END