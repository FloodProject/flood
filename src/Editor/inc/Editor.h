/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "SceneTreeCtrl.h"
#include "ResourceTreeCtrl.h"

namespace vapor { namespace editor {

//-----------------------------------//

class Plugin;
class PluginManager;
class PluginManagerFrame;
class EditorInputManager;
class UndoManager;
class Viewframe;

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

protected:

	// Initializes the engine and editor.
	void initEngine();
	void waitFinishLoad();

	// Creates the layout of the editor.
	void createNotebook();
	void createMenus();
	void createToolbar();
	void createStatusbar();
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
	void OnNodeSelected(wxTreeItemId old, wxTreeItemId id);

	// wxWidgets Input events.
	void OnKeyDown(wxKeyEvent& event);
	void OnKeyUp(wxKeyEvent& event);
	void OnMouseEvent(wxMouseEvent& event);

	// Main engine instance.
	Engine* engine;

	// Manages the editor scene entities.
	ScenePtr editorScene;

	// Input Management.
	EditorInputManager* im;

	// Manages the plugins.
	PluginManager* pluginManager;
	PluginManagerFrame* pluginManagerFrame;

	// Manages the undo/redo operations.
	UndoManager* undoManager;

	// UI widgets.
	wxBoxSizer* sizer;
	wxToolBar* toolBar;
	Viewframe* viewframe;
	wxNotebook* notebookCtrl;
	SceneTreeCtrl* sceneTreeCtrl;
	ResourceTreeCtrl* resourceTreeCtrl;

	LightPtr light;

private:

    // any class wishing to process wxWidgets events must use this macro
    DECLARE_EVENT_TABLE()
};

//-----------------------------------//

} } // end namespaces