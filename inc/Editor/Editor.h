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
#include "Core/PluginManager.h"
#include "DocumentManager.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

class EditorFrame;

class EditorApp : public wxApp
{
public:

	bool OnInit() OVERRIDE;

#ifndef BUILD_DEBUG
	void OnFatalException() OVERRIDE;
#endif

	EditorFrame* mainFrame;
};

//-----------------------------------//

class EventManager;
class Document;
class DocumentManager;
class EditorInputManager;
class PluginManager;
class Engine;

class API_EDITOR EditorFrame : public wxFrame
{
public:

	EditorFrame(const wxString& title);
	virtual ~EditorFrame();
	
	// Gets the notebook control.
	GETTER(Notebook, wxAuiNotebook*, notebookCtrl)

	// Gets the toolbar control.
	GETTER(Toolbar, wxAuiToolBar*, toolbarCtrl)

	// Gets the status bar control.
	GETTER(Statusbar, wxStatusBar*, statusCtrl)

	// Gets the AUI interface manager.
	GETTER(AUI, wxAuiManager*, paneCtrl)

	// Gets the Undo/Redo manager instance.
	GETTER(PluginManager, PluginManager*, pluginManager)

	// Gets the events manager instance.
	GETTER(EventManager, EventManager*, eventManager)

	// Gets the documents manager instance.
	GETTER(DocumentManager, DocumentManager*, documentManager)

	// Gets the current document.
	Document* getDocument() const;

	// Gets/sets the drag and drop coords.
	ACESSOR(DropCoords, Vector2, dropCoords)

	// Gets a document from a page.
	Document* getDocumentFromPage(int selection);

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
	void OnClose(wxCloseEvent& event);
    void OnAbout(wxCommandEvent& event);
	void OnAboutWx(wxCommandEvent& event);
	void OnPreferences(wxCommandEvent& event);
	void OnToolbarButtonClick(wxCommandEvent& event);
	void onNotebookPageChanged(wxAuiNotebookEvent& event);
	void onNotebookPageClose(wxAuiNotebookEvent& event);

	// wxWidgets menu events.
	void OnMenuOpenEvent(wxMenuEvent& event);
    void OnMenuExitEvent(wxCommandEvent& event);
	void OnPanelsMenuEvent(wxCommandEvent& event);
	void OnPanelsMenuUpdate(wxUpdateUIEvent& event);
	void OnSettingsRender(wxCommandEvent& event);
	void OnSettingsRenderUpdate(wxUpdateUIEvent& event);

	// Document callbacks.
	void onDocumentAdded(Document* document);
	void onDocumentRemoved(Document* document);
	void onDocumentRenamed(Document* document);

public:

	Engine* engine;
	PluginManager* pluginManager;
	DocumentManager* documentManager;
	EventManager* eventManager;
	
	// Drag and drop coordinates.
	Vector2 dropCoords;

	// Docking widgets.
	wxAuiManager* paneCtrl;
	wxAuiToolBar* toolbarCtrl;
	wxAuiNotebook* notebookCtrl;
	wxStatusBar* statusCtrl;

	// Menu widgets.
	wxMenu* menuFile;
	wxMenu* menuEdit;
	wxMenu* menuTools;
	wxMenu* menuPanels;
	wxMenu* menuSettings;
	wxMenu* menuServer;
	wxMenu* menuHelp;

	wxKeyProfileArray keyProfiles;
};

// Gets the editor instance.
API_EDITOR EditorFrame& GetEditor();

template<typename T>
T* GetPlugin()
{
	PluginManager* pm = GetEditor().getPluginManager();
	return (T*) pm->getPluginFromClass(T::getStaticType());
}

//-----------------------------------//

NAMESPACE_EDITOR_END