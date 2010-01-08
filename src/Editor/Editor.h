/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once
 
#include "vaporControl.h"
#include "SceneTreeCtrl.h"
#include "ResourceTreeCtrl.h"
#include "ConsoleFrame.h"
#include "Viewport.h"
#include <wx/notebook.h>

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
public:
    // ctor(s)
    EditorFrame(const wxString& title);
	virtual ~EditorFrame();

protected:

	// Create the editor Scene.
	void createScene();

	// create notebook with tabs
	void createNotebook();

	// creates and populates the menus
	void createMenus();

	// creates and populates the main toolbar
	void createToolbar();

	// creates the statusbar
	void createStatusbar();

	// vaporEngine-related stuff
	void initEngine();

    // event handlers (these functions should _not_ be virtual)
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
	void OnToolbarButtonClick(wxCommandEvent& event);

	// Handles picking of entities.
	void onMouseClick( const vapor::input::MouseButtonEvent& mbe );

	// vaporEngine instance.
	vapor::Engine* engine;
	
	// Manages the Scene entities.
	SceneTreeCtrl* sceneTreeCtrl;

	// Manages the Resource files.
	ResourceTreeCtrl* resourceTreeCtrl;

	// Groups the above controllers.
	wxNotebook* notebookCtrl;

	// vaporEngine's wxWidgets control.
	vaporControl* vaporCtrl;

	// Main layout sizer.
	wxBoxSizer* sizer;

	Viewport* viewport;

	ConsoleFrame* codeEvaluator;

	std::vector< scene::NodePtr > selectedNodes;

private:

    // any class wishing to process wxWidgets events must use this macro
    DECLARE_EVENT_TABLE()

	void fireCube(Vector3 direction, Vector3 pos, ScenePtr scene, MS3DPtr mesh);
};

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// IDs for the controls and the menu commands
enum
{
	Camera_Viewport,

    // menu items
    Editor_Quit = wxID_EXIT,

    // it is important for the id corresponding to the "About" command to have
    // this standard value as otherwise it won't be handled properly under Mac
    // (where it is special and put into the "Apple" menu)
    Editor_About = wxID_ABOUT,

	// Toolbar buttons
	Toolbar_ToggleScene,
	Toolbar_ToogleConsole,
	Toolbar_ToogleGrid,
	Toolbar_TooglePlay,
};

//-----------------------------------//

} } // end namespaces