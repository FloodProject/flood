/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once
 
// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"
 
// for all others, include the necessary headers (this file is usually all 
//  you need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include <wx/treectrl.h> 

#include <vapor/Engine.h>

/**
 * This control is responsible for mantaining and updating a TreeCtrl
 * with all the contents of the scene tree in the passed instance of
 * the engine. The events of this control (right-click context menus)
 * will be used to change some settings of the scene tree, and also
 * properties of the associated scene nodes.
 */

class SceneTreeCtrl : public wxTreeCtrl 
{
public:

	SceneTreeCtrl(vapor::Engine* engine, 
					wxWindow* parent, wxWindowID id = wxID_ANY,
					const wxPoint& pos	= wxDefaultPosition,
					const wxSize& size = wxDefaultSize,
					long style = wxTR_DEFAULT_STYLE,
					const wxValidator& validator = wxDefaultValidator, 
					const wxString&	name = "SceneTreeCtrl");
					
	virtual ~SceneTreeCtrl();

//-----------------------------------//

	wxImageList* imageList;

	vapor::Engine* getEngine() { return engine; }
	void setEngine(vapor::Engine* engine) { this->engine = engine; }

	//-----------------------------------//
	// EVENT HANDLERS                    
	//-----------------------------------//
	
	void onItemMenu(wxTreeEvent& event);

protected:

	void initControl();

	void initIcons();

	// scene associated this control
	vapor::scene::ScenePtr scene;

	// holds the engine instance
	vapor::Engine* engine;

	DECLARE_EVENT_TABLE()
};

enum 
{
	ID_SceneTree,
	ID_MenuSceneNodeDelete = wxID_DELETE
};
