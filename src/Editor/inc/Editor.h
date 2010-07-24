/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once
 
#include "UndoOperation.h"
#include "Viewframe.h"
#include "SceneTreeCtrl.h"
#include "ResourceTreeCtrl.h"
#include "PluginManager.h"

namespace vapor { namespace editor {

class Plugin;
class PluginManagerFrame;
class EditorInputManager;

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
	friend class Plugin;

public:

    EditorFrame(const wxString& title);
	virtual ~EditorFrame();

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

	// Undo/Redo operations.
	void registerOperation( UndoOperation* const op );
	void handleUndoRedoOperation(Operations&, Operations&, bool undo);
	void updateUndoRedoUI();
	
	void onUpdate( double delta );
	void onRender();
	void RefreshViewport();

	GETTER(MainViewframe, Viewframe* const, viewframe)
	GETTER(Engine, Engine* const, engine)
	GETTER(EditorScene, ScenePtr, editorScene)

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

	NodePtr createCamera();
	void createMainViewframe();

	// Main engine instance.
	Engine* engine;

	// Manages the editor scene entities.
	ScenePtr editorScene;

	// Input Management
	EditorInputManager* im;

	// Manages the tools.
	PluginManager* pluginManager;
	PluginManagerFrame* pluginManagerFrame;

	// Undo/redo operations.
	Operations undoOperations;
	Operations redoOperations;

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