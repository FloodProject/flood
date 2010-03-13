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

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// IDs for the controls and the menu commands
enum
{
    // menu items
    Editor_Quit = wxID_EXIT,

    // it is important for the id corresponding to the "About" command to have
    // this standard value as otherwise it won't be handled properly under Mac
    // (where it is special and put into the "Apple" menu)
    Editor_About = wxID_ABOUT,

	// Toolbar buttons
	Toolbar_Save = 12879,
	Toolbar_ToggleScene,
	Toolbar_ToogleConsole,
	Toolbar_ToogleGrid,
	Toolbar_TooglePlay,
	Toolbar_ToogleSidebar,

	Toolbar_Undo,
	Toolbar_Redo
};

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

	void onCameraTransform();
	void RefreshCanvas();

protected:

	// Creates the layout of the editor.
	void createScene();
	void createNotebook();
	void createMenus();
	void createToolbar();
	void createStatusbar();
	void createModes();

	// Initializes vapor3D engine.
	void initEngine();

public:

	// 3D engine instance.
	Engine* engine;

	// Editor modes.
	Mode* currentMode;
	std::map<int, Mode*> modeIds;
	std::vector<Mode*> editorModes;

	// Saves all the operations in a stack so you can undo
	// any editing operation you've done while editing.
	std::stack<Operation*> operations;

	// Main UI widgets.
	wxBoxSizer* sizer;
	wxToolBar* toolBar;

	// Rendering UI controls.
	Viewport* viewport;
	vaporControl* vaporCtrl;

	// Property controls.
	wxNotebook* notebookCtrl;
	SceneTreeCtrl* sceneTreeCtrl;
	ResourceTreeCtrl* resourceTreeCtrl;
	TerrainPage* terrainPage;
	ConsoleFrame* codeEvaluator;
	
	bool needsRedraw;

private:

    // any class wishing to process wxWidgets events must use this macro
    DECLARE_EVENT_TABLE()
};

//-----------------------------------//

} } // end namespaces