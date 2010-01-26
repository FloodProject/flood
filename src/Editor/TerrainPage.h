/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once
 
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
		const wxSize& size = wxDefaultSize, 
		long style = wxTAB_TRAVERSAL );
	
	//~TerrainPage();

protected:

	wxChoicebook* m_cbTerrainTool;
	wxPanel* m_panelBrush;
	wxStaticLine* m_staticline5;
	wxStaticText* m_staticText2;
	wxChoice* m_choiceBrush;
	wxStaticText* m_staticText31;
	wxSlider* m_sliderSize;
	wxTextCtrl* m_textCtrlSize;
	wxStaticText* m_staticText3;
	wxSlider* m_sliderStrength;
	wxTextCtrl* m_textCtrlStrength;
	
	// Virtual event handlers, overide them in your derived class
	virtual void OnToolChoice( wxCommandEvent& event ){ event.Skip(); }

	DECLARE_EVENT_TABLE()
};

//-----------------------------------//

enum 
{

};

//-----------------------------------//

} } // end namespaces