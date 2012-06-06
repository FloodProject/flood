/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "EditorPlugin.h"
#include "Project.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

class SceneDocument;
class ProjectPane;

REFLECT_DECLARE_CLASS(ProjectPlugin)

class ProjectPlugin : public EditorPlugin
{
	REFLECT_DECLARE_OBJECT(ProjectPlugin)

public:

	// Gets metadata about this plugin.
	PluginMetadata getMetadata() OVERRIDE;

	// Plugin callbacks.
	void onPluginEnable() OVERRIDE;
	void onPluginDisable() OVERRIDE;

	// Command callbacks.
	void onNewDocument(wxCommandEvent& event);
	void onOpenDocument(wxCommandEvent& event);
	void onSaveDocument(wxCommandEvent& event);
	void onSaveAsDocument(wxCommandEvent& event);

	// Creates a new document.
	Document* createDocument();

	// Creates a new project.
	Project* createProject(const String& name);

	// New toolbar button.
	wxAuiToolBarItem* newButton;
	wxMenuItem* newItem;

	// Open toolbar button.
	wxAuiToolBarItem* openButton;
	wxMenuItem* openItem;

	// Save toolbar button.
	wxAuiToolBarItem* saveButton;
	wxMenuItem* saveItem;
	wxMenuItem* saveAsItem;

	ProjectPane* projectPane;

protected:

	// Asks to save unsaved changes.
	bool askSaveChanges(Document* document);
};

//-----------------------------------//

NAMESPACE_EDITOR_END