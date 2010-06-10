/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once
 
#include "Mode.h"
#include "Operation.h"
#include "Viewframe.h"
#include "SceneTreeCtrl.h"
#include "ResourceTreeCtrl.h"
#include "TerrainPage.h"
#include "ConsoleFrame.h"

namespace vapor { namespace editor {

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
	friend class Mode;

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

	// Engine Input events.
	void onMouseMove( const MouseMoveEvent& );
	void onMouseDrag( const MouseDragEvent& );
	void onMousePress( const MouseButtonEvent& );
	void onMouseRelease( const MouseButtonEvent& );
	void onMouseEnter();
	void onMouseLeave();
	void onKeyPress( const KeyEvent& );
	void onKeyRelease( const KeyEvent& );

	// Misc stuff.
	void onModeSwitch( Mode* const mode, int id );
	void registerOperation( Operation* const op );
	void updateUndoRedoUI();
	
	void onUpdate( double delta );
	void onRender();
	void RefreshViewport();

	IMPLEMENT_GETTER(MainViewframe, Viewframe* const, viewframe)
	IMPLEMENT_GETTER(Engine, Engine* const, engine)
	IMPLEMENT_GETTER(EditorScene, ScenePtr, editorScene)

protected:

	// Initializes the engine and editor.
	void initEngine();
	void waitFinishLoad();

	// Creates the layout of the editor.
	void createNotebook();
	void createMenus();
	void createToolbar();
	void createStatusbar();
	void createModes();

	// Creates the default scenes.
	void createEditorScene();
	void createScene();

	NodePtr createCamera();
	void createMainViewframe();

	// Main engine instance.
	Engine* engine;
	ScenePtr editorScene;

	// Input Management
	vaporInputManager* im;
	//InputProcessCallback cb;
	//void onInputEvent( Event& );

	// Editor modes.
	Mode* currentMode;
	std::vector<Mode*> modes;
	std::map<int, Mode*> modesMap;

	// Undo/redo operations.
	typedef std::deque<Operation*> Operations;
	Operations undoOperations;
	Operations redoOperations;

	// UI widgets.
	wxBoxSizer* sizer;
	wxToolBar* toolBar;
	Viewframe* viewframe;
	wxNotebook* notebookCtrl;
	SceneTreeCtrl* sceneTreeCtrl;
	ResourceTreeCtrl* resourceTreeCtrl;
	TerrainPage* terrainPage;
	ConsoleFrame* codeEvaluator;

	LightPtr light;

private:

    // any class wishing to process wxWidgets events must use this macro
    DECLARE_EVENT_TABLE()
};

//-----------------------------------//

} } // end namespaces