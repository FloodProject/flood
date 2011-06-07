/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#ifdef ENABLE_PLUGIN_TERRAIN
 
#include "wxSliderCtrl.h"
#include "wxImageComboBox.h"
#include "wx/bmpcbox.h"

#define BRUSH_INITIAL_SIZE 100

namespace vapor { namespace editor {

//-----------------------------------//

class TerrainPage : public wxScrolledWindow
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
	ImageHandle getBrushImage();

	// Gets the paint image.
	ImagePtr getPaintImage();

	// Gets the state of tile lock.
	bool getTileLock() { return m_tileLock->GetValue(); }

	// Gets the offset of tile lock.
	int32 getTileOffsetX() { return m_tileOffsetX->GetSlider()->GetValue(); }
	int32 getTileOffsetY() { return m_tileOffsetY->GetSlider()->GetValue(); }

protected:

	void createUI();
	void createBrush();
	void createCell();
	void createBrushes();

	void onBrushDropdown(wxCommandEvent& event);
	void onBrushSelected(wxCommandEvent& event);

	void onTextureDropdown(wxCommandEvent& event);
	void onTextureSelected(wxCommandEvent& event);

	wxChoicebook* m_cbTerrainTool;
	//wxScrolledWindow* scrolledWindow;
	
	wxPanel* m_panelBrush;
	wxPanel* m_panelCell;
	
	wxBitmapComboBox* m_choiceBrush;
	wxSliderCtrl* m_sliderSize;
	wxSliderCtrl* m_sliderStrength;
	wxComboBox* m_textureChoice;
	wxCheckBox* m_tileLock;
	wxSliderCtrl* m_tileOffsetX;
	wxSliderCtrl* m_tileOffsetY;

	// Caches the brush image.
	ImageHandle brushHandle;

	// Virtual event handlers, overide them in your derived class
	virtual void OnToolChoice( wxCommandEvent& event ){ event.Skip(); }
};

//-----------------------------------//

} } // end namespaces

#endif