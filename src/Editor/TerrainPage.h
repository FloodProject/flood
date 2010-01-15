/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once
 
#include <wx/choicebk.h> 

namespace vapor { namespace editor {

//-----------------------------------//

/**
 * Notebook tab that manages terrains.
 */

class TerrainPage : public wxChoicebook 
{
	public:

		TerrainPage( wxWindow* parent, wxWindowID id = wxID_ANY, 
			const wxPoint& pos = wxDefaultPosition, 
			const wxSize& size = wxSize( 251,454 ), 
			long style = wxCHB_DEFAULT );
		
		~TerrainPage();
	
	protected:
		//wxStaticText* m_staticText1;
		//wxComboBox* toolChoice;
		//wxStaticLine* m_staticline1;
		//wxStaticText* m_staticText2;
		//wxChoice* brushChoice;
		//wxStaticText* m_staticText31;
		//wxSlider* m_slider11;
		//wxTextCtrl* m_textCtrl11;
		//wxStaticText* m_staticText3;
		//wxSlider* m_slider1;
		//wxTextCtrl* m_textCtrl1;
		
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