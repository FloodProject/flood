/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

namespace vapor { namespace editor {

//-----------------------------------//

class Plugin;
class PluginManager;
class PluginManagerFrame;
class EditorInputManager;
class UndoManager;
class Viewframe;
class Events;

//-----------------------------------//

// Define a new application type, each program should derive a class from wxApp
class EditorApp : public wxApp
{
public:

    // return: if OnInit() returns false, the application terminates)
    virtual bool OnInit();
};

//-----------------------------------//

// Define a new frame type: this is going to be our main frame
class EditorFrame : public wxFrame
{
public:

    EditorFrame(const wxString& title);
	virtual ~EditorFrame();
	
	// Refreshes the main viewport.
	void RefreshViewport();

	// Gets the engine instance.	
	GETTER(Engine, Engine* const, engine)

	// Gets the main viewframe instance.
	GETTER(MainViewframe, Viewframe* const, viewframe)

	// Gets the editor scene instance.
	GETTER(EditorScene, ScenePtr, editorScene)

	// Gets the Undo/Redo manager instance.
	GETTER(UndoManager, UndoManager*, undoManager)

	// Gets the Undo/Redo manager instance.
	GETTER(PluginManager, PluginManager*, pluginManager)

	// Gets the events manager instance.
	GETTER(EventManager, Events*, eventManager)

	// Gets the notebook control.
	GETTER(Notebook, wxNotebook*, notebookCtrl)

	// Gets the toolbar control.
	GETTER(Toolbar, wxToolBar*, toolBar)

protected:

	// Initializes the engine and editor.
	void initEngine();
	void waitFinishLoad();

	// Creates the layout of the editor.
	void createNotebook();
	void createMenus();
	void createToolbar();
	void createPlugins();

	// Creates the default scenes.
	void createEditorScene();
	void createScene();
	void createMainViewframe();
	NodePtr createCamera();

	// Viewframe callbacks.
	void onUpdate( double delta );
	void onRender();

    // wxWidgets events.
	void OnIdle(wxIdleEvent& event);
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
	void OnToolbarButtonClick(wxCommandEvent& event);

	// wxWidgets Input events.
	void OnKeyDown(wxKeyEvent& event);
	void OnKeyUp(wxKeyEvent& event);
	void OnMouseEvent(wxMouseEvent& event);

	// Main engine instance.
	Engine* engine;

	// Manages the editor scene entities.	
	ScenePtr editorScene;

	// Input Management.
	EditorInputManager* inputManager;

	// Manages the plugins.
	PluginManager* pluginManager;
	PluginManagerFrame* pluginManagerFrame;

	// Manages the undo/redo operations.
	UndoManager* undoManager;
	Events* eventManager;

	// UI widgets.
	wxBoxSizer* sizer;
	wxToolBar* toolBar;
	Viewframe* viewframe;
	wxNotebook* notebookCtrl;

private:

    // any class wishing to process wxWidgets events must use this macro
    DECLARE_EVENT_TABLE()
};

//-----------------------------------//

} } // end namespaces