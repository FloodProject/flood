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

#include <wx/notebook.h> 

namespace vapor { namespace editor {

//-----------------------------------//

/**
 * Notebook tab that manages terrains.
 */

class TerrainPage : public wxPanel 
{
	public:

		TerrainPage( wxWindow* parent, wxWindowID id = wxID_ANY, 
			const wxPoint& pos = wxDefaultPosition, 
			const wxSize& size = wxSize( 251,454 ), 
			long style = wxTAB_TRAVERSAL );
		
		~TerrainPage();
	
	protected:
		wxStaticText* m_staticText1;
		wxComboBox* toolChoice;
		wxStaticLine* m_staticline1;
		wxStaticText* m_staticText2;
		wxChoice* brushChoice;
		wxStaticText* m_staticText31;
		wxSlider* m_slider11;
		wxTextCtrl* m_textCtrl11;
		wxStaticText* m_staticText3;
		wxSlider* m_slider1;
		wxTextCtrl* m_textCtrl1;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnToolChoice( wxCommandEvent& event ){ event.Skip(); }

	DECLARE_EVENT_TABLE()
};

//-----------------------------------//

enum 
{
	ID_SceneTree,
	ID_MenuSceneNodeDelete = wxID_DELETE,
	ID_MenuSceneNodeVisible,
	ID_MenuSceneNodeWireframe,
};

//-----------------------------------//

} } // end namespaces