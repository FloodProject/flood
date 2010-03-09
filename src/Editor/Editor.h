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
    // override base class virtuals
    // ----------------------------

    // this one is called on application startup and is a good place for the app
    // initialization (doing it here and not in the ctor allows to have an error
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

    // event handlers (these functions should _not_ be virtual)
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
	void OnToolbarButtonClick(wxCommandEvent& event);
	void OnKeyDown(wxKeyEvent& event);
	void OnNodeSelected(wxTreeItemId old, wxTreeItemId id);

	void onMouseClick( const input::MouseButtonEvent& mbe );
	void onKeyPress(const input::KeyEvent& key);
	void onKeyRelease(const input::KeyEvent& key);

	void onModeSwitch( Mode* newMode, int id );

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

	// Main layout sizer.
	wxBoxSizer* sizer;
	wxToolBar* toolBar;

	// Outputs vapor rendering.
	Viewport* viewport;
	vaporControl* vaporCtrl;

	// Sidebar controls.
	wxNotebook* notebookCtrl;
	SceneTreeCtrl* sceneTreeCtrl;
	ResourceTreeCtrl* resourceTreeCtrl;
	TerrainPage* terrainPage;
	
	// Lua evaluator.
	ConsoleFrame* codeEvaluator;
	
	// Editor modes
	Mode* currentMode;
	std::map<int, Mode*> modeIds;
	std::vector<Mode*> editorModes;

	// Saves all the operations in a stack so you can undo
	// any editing operation you've done while editing.
	std::stack<Operation*> operations;

private:

    // any class wishing to process wxWidgets events must use this macro
    DECLARE_EVENT_TABLE()
};

//-----------------------------------//

} } // end namespaces