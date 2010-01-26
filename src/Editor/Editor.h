/************************************************************************
*
* vaporEngine (2008-2010)
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
#include "TerrainPage.h"

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
	void onKeyPress(const vapor::input::KeyEvent& key);
	void onKeyRelease(const vapor::input::KeyEvent& key);

	// vaporEngine instance.
	vapor::Engine* engine;

	// vaporEngine's wxWidgets control.
	vaporControl* vaporCtrl;

	// Main layout sizer.
	wxBoxSizer* sizer;

	// Groups the above controllers.
	wxNotebook* notebookCtrl;
	
	// Manages the Scene entities.
	SceneTreeCtrl* sceneTreeCtrl;

	// Manages the Resource files.
	ResourceTreeCtrl* resourceTreeCtrl;

	Viewport* viewport;
	ConsoleFrame* codeEvaluator;
	TerrainPage* terrainPage;

	std::vector< scene::NodePtr > selectedNodes;

private:

    // any class wishing to process wxWidgets events must use this macro
    DECLARE_EVENT_TABLE()
};

//-----------------------------------//

} } // end namespaces