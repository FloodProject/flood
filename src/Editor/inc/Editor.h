/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include <wx/keybinder.h>
#include "Math/Vector.h"

FWD_DECL(Engine)

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

class Plugin;
class PluginManager;

class Events;
class Document;
class EditorInputManager;

//-----------------------------------//

class EditorApp : public wxApp
{
public:

    virtual bool OnInit() OVERRIDE;
	virtual void OnFatalException() OVERRIDE;
};

//-----------------------------------//

class API_EDITOR EditorFrame : public wxFrame
{
	//wxADD_KEYBINDER_SUPPORT();

public:

    EditorFrame(const wxString& title);
	virtual ~EditorFrame();
	
	// Refreshes the main view.
	void redrawView();

	// Adds a new document to the notebook.
	void addDocument(Document* document);

	// Gets the current document.
	GETTER(Document, Document*, currentDocument)

	// Gets the notebook control.
	GETTER(Notebook, wxAuiNotebook*, notebookCtrl)

	// Gets the toolbar control.
	GETTER(Toolbar, wxAuiToolBar*, toolbarCtrl)

	// Gets the AUI interface manager.
	GETTER(AUI, wxAuiManager*, paneCtrl)

	// Gets the Undo/Redo manager instance.
	GETTER(PluginManager, PluginManager*, pluginManager)

	// Gets the events manager instance.
	GETTER(EventManager, Events*, eventManager)

	// Gets/sets the drag and drop coords.
	ACESSOR(DropCoords, Vector2, dropCoords)

protected:

	// Creates the main UI layout.
	void createUI();
	void createToolbar();	
	void createMenus();
	void createLastUI();
	void createEngine();
	void createPlugins();

    // wxWidgets main events.
	void OnIdle(wxIdleEvent& event);
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
	void OnAboutWx(wxCommandEvent& event);
	void OnPreferences(wxCommandEvent& event);
	void OnToolbarButtonClick(wxCommandEvent& event);
	void onNotebookPageChanged(wxAuiNotebookEvent& event);
	void onNotebookPageClose(wxAuiNotebookEvent& event);

	// wxWidgets menu events.
	void OnMenuOpenEvent(wxMenuEvent& event);
	void OnPanelsMenuEvent(wxCommandEvent& event);
	void OnPanelsMenuUpdate(wxUpdateUIEvent& event);
	void OnSettingsRender(wxCommandEvent& event);
	void OnSettingsRenderUpdate(wxUpdateUIEvent& event);

public:

	Engine* engine;
	PluginManager* pluginManager;
	Events* eventManager;
	
	// Drag and drop coordinates.
	Vector2 dropCoords;

	Document* currentDocument;
	std::vector<Document*> documents;

	// Gets a document from a page.
	Document* getDocumentFromPage(int selection);

	// Docking widgets.
	wxAuiManager* paneCtrl;
	wxAuiToolBar* toolbarCtrl;
	wxAuiNotebook* notebookCtrl;

	// Menu widgets.
	wxMenu* menuFile;
	wxMenu* menuEdit;
	wxMenu* menuTools;
	wxMenu* menuPanels;
	wxMenu* menuSettings;
	wxMenu* menuHelp;

	wxKeyProfileArray keyProfiles;
};

// Gets the editor instance.
API_EDITOR EditorFrame& GetEditor();

//-----------------------------------//

NAMESPACE_EDITOR_END