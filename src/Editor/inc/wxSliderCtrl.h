/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

namespace vapor { namespace editor {

//-----------------------------------//

/**
 * Simple widget that integrates a slider with a text box. This way you
 * can easily see what value the slider is set to, and change it via
 * keyboard input in the text box.
 */

class wxSliderCtrl : public wxPanel
{
public:
	
	wxSliderCtrl( wxWindow* parent, wxWindowID id,
		int initial, int min, int max );

	wxSlider* GetSlider() const { return m_slider; }

private:

	void onScroll(wxScrollEvent& event);
	void onTextEnter(wxCommandEvent& event);

	wxSlider* m_slider;
	wxTextCtrl* m_textCtrl;
	
	DECLARE_EVENT_TABLE()
};

//-----------------------------------//

} } // end namespaces