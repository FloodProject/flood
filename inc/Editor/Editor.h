/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Editor/API.h"
#include "Graphics/RenderTarget.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

class EditorFrame;

class EditorApp : public wxApp
{
public:

	bool OnInit() override;
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

	SceneWindow* sceneWindow;
};

// Gets the editor instance.
API_EDITOR EditorFrame& GetEditor();

//-----------------------------------//

NAMESPACE_EDITOR_END