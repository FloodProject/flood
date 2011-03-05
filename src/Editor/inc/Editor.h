/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Math/Vector2.h"
FWD_DECL(Engine)

namespace vapor { namespace editor {

//-----------------------------------//

class Plugin;
class PluginManager;
class PluginManagerFrame;
class EditorInputManager;
class Events;
class Document;

//-----------------------------------//

class EditorApp : public wxApp
{
public:

    virtual bool OnInit();
	virtual void OnFatalException();
};

//-----------------------------------//

class EditorFrame : public wxFrame
{
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

	// Gets the engine instance.	
	GETTER(Engine, Engine*, engine)

	// Gets/sets the drag and drop coords.
	ACESSOR(DropCoords, Vector2i, dropCoords)

protected:

	// Creates the main UI layout.
	void createUI();
	void createLastUI();
	void createMenus();
	void createToolbar();
	void createNotebook();
	void createEngine();
	void createServices();
	void createPlugins();

    // wxWidgets main events.
	void OnIdle(wxIdleEvent& event);
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
	void OnAboutWx(wxCommandEvent& event);
	void OnToolbarButtonClick(wxCommandEvent& event);
	void onNotebookPageChanged(wxAuiNotebookEvent& event);
	void onNotebookPageClose(wxAuiNotebookEvent& event);

	// wxWidgets menu events.
	void OnMenuOpenEvent(wxMenuEvent& event);
	void OnPanelsMenuEvent(wxCommandEvent& event);
	void OnPanelsMenuUpdate(wxUpdateUIEvent& event);
	void OnSettingsRender(wxCommandEvent& event);
	void OnSettingsRenderUpdate(wxUpdateUIEvent& event);

#if 0
	// Gets the player camera.
	CameraPtr getPlayerCamera() const;

	// Switches the editor to play mode.
	void switchPlayMode(bool switchToPlay);
#endif

	Engine* engine;

	// Plugins.
	PluginManager* pluginManager;
	PluginManagerFrame* pluginManagerFrame;
	Events* eventManager;
	
	// Drag and drop coordinates.
	Vector2i dropCoords;

	// UI widgets.
	wxAuiManager* paneCtrl;
	wxAuiToolBar* toolbarCtrl;
	wxAuiNotebook* notebookCtrl;

public:

	// Gets a document from a page.
	Document* getDocumentFromPage(int selection);

	Document* currentDocument;
	std::vector<Document*> documents;

	wxMenu* menuFile;
	wxMenu* menuEdit;
	wxMenu* menuTools;
	wxMenu* menuPanels;
	wxMenu* menuSettings;
	wxMenu* menuHelp;
};

// Gets the editor instance.
EditorFrame& GetEditor();

//-----------------------------------//

} } // end namespaces