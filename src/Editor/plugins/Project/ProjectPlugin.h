/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Plugin.h"

namespace vapor { namespace editor {

//-----------------------------------//

class ResourceDatabase;

REFLECT_DECLARE_CLASS(Project)

class Project : public Object
{
	REFLECT_DECLARE_OBJECT(Project)

public:
	
	ResourceDatabase* database;
};

//-----------------------------------//

class SceneDocument;

REFLECT_DECLARE_CLASS(ProjectPlugin)

class ProjectPlugin : public Plugin
{
	REFLECT_DECLARE_OBJECT(ProjectPlugin)

public:

	ProjectPlugin();

	// Gets metadata about this plugin.
	virtual PluginMetadata getMetadata();

	// Plugin enable callback.
	virtual void onPluginEnable();

	// Plugin disable callback.
	virtual void onPluginDisable();

	// Handles new button click.
	void onNewButtonClick(wxCommandEvent& event);

	// Handles open button click.
	void onOpenButtonClick(wxCommandEvent& event);

	// Handles save button click.
	void onSaveButtonClick(wxCommandEvent& event);

	// Updates the save button UI state.
	void onSaveButtonUpdateUI(wxUpdateUIEvent& event);

	// New toolbar button.
	wxAuiToolBarItem* newButton;
	wxMenuItem* newItem;

	// Open toolbar button.
	wxAuiToolBarItem* openButton;
	wxMenuItem* openItem;

	// Save toolbar button.
	wxAuiToolBarItem* saveButton;
	wxMenuItem* saveItem;

protected:

	// Switches the scene.
	void switchScene(SceneDocument* document);

	// Asks to save unsaved changes.
	bool askSaveChanges();

	// Saves the current scene.
	bool saveScene();
};

//-----------------------------------//

} } // end namespaces