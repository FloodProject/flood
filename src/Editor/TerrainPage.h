/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once
 
#include "wxSliderCtrl.h"
#include "wxImageComboBox.h"

namespace vapor { namespace editor {

//-----------------------------------//

/**
 * Notebook tab that manages terrains.
 */

class TerrainPage : public wxPanel 
{
public:

	TerrainPage( Engine* engine, wxWindow* parent, wxWindowID id = wxID_ANY, 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, 
		long style = wxTAB_TRAVERSAL );

	void createUI();

protected:

	wxChoicebook* m_cbTerrainTool;
	wxPanel* m_panelBrush;
	wxStaticLine* m_staticline5;
	wxStaticText* m_staticText2;
	wxImageComboBox* m_choiceBrush;
	wxStaticText* m_staticText31;
	wxSliderCtrl* m_sliderSize;
	wxStaticText* m_staticText3;
	wxSliderCtrl* m_sliderStrength;
	
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