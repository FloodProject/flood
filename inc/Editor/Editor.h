/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

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

class SceneWindow;
class EditorInputManager;
class InputManager;
class Engine;
struct Archive;

class API_EDITOR EditorFrame : public wxFrame
{
public:

	EditorFrame(const wxString& title);
	virtual ~EditorFrame();
	
	// Gets the AUI interface manager.
	GETTER(AUI, wxAuiManager*, paneCtrl)

protected:

	// Creates the main UI layout.
	void createUI();
	void createEngine();

	void onRender();
	void onUpdate(float);
	void onResize(const Settings& settings);

	// wxWidgets main events.
	void OnIdle(wxIdleEvent& event);
	void OnClose(wxCloseEvent& event);
	void OnSize(wxSizeEvent& event);
	void OnSizing(wxSizeEvent& event);

public:

	Engine* engine;
	Archive* archive;
	InputManager* input;

	// Docking widgets.
	wxAuiManager* paneCtrl;
	SceneWindow* sceneWindow;
};

// Gets the editor instance.
API_EDITOR EditorFrame& GetEditor();

//-----------------------------------//

NAMESPACE_EDITOR_END