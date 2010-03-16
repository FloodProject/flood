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
#include "Viewport.h"
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

    // wxWidgets events
	void OnIdle(wxIdleEvent& event);
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
	void OnToolbarButtonClick(wxCommandEvent& event);
	void OnKeyDown(wxKeyEvent& event);
	void OnNodeSelected(wxTreeItemId old, wxTreeItemId id);

	// Mouse events
	void onMouseMove( const input::MouseMoveEvent& mve );
	void onMouseDrag( const input::MouseDragEvent& mde );
	void onMousePress( const input::MouseButtonEvent& mbe );
	void onMouseRelease( const input::MouseButtonEvent& mbe );
	void onMouseEnter();
	void onMouseLeave();

	// Keyboard events
	void onKeyPress( const input::KeyEvent& key );
	void onKeyRelease( const input::KeyEvent& key );

	// Mode/Undo stuff
	void onModeSwitch( Mode* newMode, int id );
	void registerOperation( Operation* const op );

	void onUpdate();
	void onRender();
	void onCameraTransform();
	void RefreshCanvas();

protected:

	// Creates the layout of the editor.
	void createEditorScene();
	void createScene();
	void createNotebook();
	void createMenus();
	void createToolbar();
	void createStatusbar();
	void createModes();

	// Initializes vapor3D engine.
	void initEngine();

public:

	Engine* engine;
	ScenePtr editorScene;
	CameraPtr editorCamera;
	
	// Editor modes.
	Mode* currentMode;
	std::vector<Mode*> modes;
	std::map<int, Mode*> modesMap;

	// Undo/redo stack.
	std::stack<Operation*> operations;

	// UI widgets.
	wxBoxSizer* sizer;
	wxToolBar* toolBar;
	Viewport* viewport;
	wxNotebook* notebookCtrl;
	SceneTreeCtrl* sceneTreeCtrl;
	ResourceTreeCtrl* resourceTreeCtrl;
	TerrainPage* terrainPage;
	ConsoleFrame* codeEvaluator;

private:

    // any class wishing to process wxWidgets events must use this macro
    DECLARE_EVENT_TABLE()
};

//-----------------------------------//

} } // end namespaces