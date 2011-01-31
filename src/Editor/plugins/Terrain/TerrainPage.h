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

class TerrainPage : public wxPanel
{
public:

	TerrainPage( wxWindow* parent, wxWindowID id = wxID_ANY, 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, 
		long style = wxTAB_TRAVERSAL );

	// Gets the strength of the brush.
	int	getBrushStrength();

	// Gets the size of the brush.
	int	getBrushSize();

	// Gets the image of the brush.
	//int getBrushImage();

	// Gets the paint image.
	ImagePtr getPaintImage();

	// Gets the state of tile lock.
	bool getTileLock() { return m_tileLock->GetValue(); }

	// Gets the offset of tile lock.
	bool getTileOffsetX() { return m_tileOffsetX->GetSlider()->GetValue(); }
	bool getTileOffsetY() { return m_tileOffsetY->GetSlider()->GetValue(); }

protected:

	void createUI();
	void createBrush();
	void createCell();
	void createBrushes();

	void onComboBoxDropdown(wxCommandEvent& event);
	void onComboBoxSelected(wxCommandEvent& event);

	wxChoicebook* m_cbTerrainTool;
	
	wxPanel* m_panelBrush;
	wxPanel* m_panelCell;
	
	wxImageComboBox* m_choiceBrush;
	wxSliderCtrl* m_sliderSize;
	wxSliderCtrl* m_sliderStrength;
	wxComboBox* m_textureChoice;
	wxCheckBox* m_tileLock;
	wxSliderCtrl* m_tileOffsetX;
	wxSliderCtrl* m_tileOffsetY;

	// Virtual event handlers, overide them in your derived class
	virtual void OnToolChoice( wxCommandEvent& event ){ event.Skip(); }
};

//-----------------------------------//

} } // end namespaces