/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Core/Math/Vector.h"
#include "Core/PluginManager.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

class EditorFrame;

class EditorApp : public wxApp
{
public:

	bool OnInit() OVERRIDE;
	EditorFrame* mainFrame;
};

//-----------------------------------//

class EditorInputManager;
class PluginManager;
class InputManager;
class Engine;
struct Archive;

class API_EDITOR EditorFrame : public wxFrame
{
public:

	EditorFrame(const wxString& title);
	virtual ~EditorFrame();
	
	// Gets the notebook control.
	GETTER(Notebook, wxAuiNotebook*, notebookCtrl)

	// Gets the AUI interface manager.
	GETTER(AUI, wxAuiManager*, paneCtrl)

protected:

	// Creates the main UI layout.
	void createUI();
	void createEngine();

	// wxWidgets main events.
	void OnIdle(wxIdleEvent& event);
	void OnClose(wxCloseEvent& event);

public:

	Engine* engine;
	PluginManager* pluginManager;
	Archive* archive;
	InputManager* input;

	// Docking widgets.
	wxAuiManager* paneCtrl;
	wxAuiToolBar* toolbarCtrl;
	wxAuiNotebook* notebookCtrl;
	wxStatusBar* statusCtrl;
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