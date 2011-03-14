/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Editor/API.h"
#include "wxSliderCtrl.h"

namespace vapor { namespace editor {

////////////////////////////////////////////////////////////
// Event table
////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(wxSliderCtrl, wxPanel)
	EVT_COMMAND_SCROLL(wxID_ANY, wxSliderCtrl::onScroll)
	EVT_TEXT_ENTER(wxID_ANY, wxSliderCtrl::onTextEnter)
END_EVENT_TABLE()

//-----------------------------------//

wxSliderCtrl::wxSliderCtrl( wxWindow* parent, wxWindowID id,
						   int initial, int min, int max)
	: wxPanel( parent, id )
{
	wxBoxSizer* bSizer81;
	bSizer81 = new wxBoxSizer( wxHORIZONTAL );
	
	m_slider = new wxSlider( this, wxID_ANY, initial, min, max,
		wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	bSizer81->Add( m_slider, 1, wxEXPAND, 5 );
	
	wxString num = wxString::Format( "%d", initial );
	m_textCtrl = new wxTextCtrl( this, wxID_ANY, num, 
		wxDefaultPosition, wxSize( 30,-1 ), wxTE_PROCESS_ENTER );
	bSizer81->Add( m_textCtrl, 0, wxEXPAND, 5 );
	
	SetSizerAndFit( bSizer81 );
}

//-----------------------------------//

void wxSliderCtrl::onScroll(wxScrollEvent& event)
{
	wxString num = wxString::Format( "%d", event.GetPosition() );
	m_textCtrl->ChangeValue(num);
}

//-----------------------------------//

void wxSliderCtrl::onTextEnter(wxCommandEvent&)
{
	int num = wxAtoi( m_textCtrl->GetValue() );
	m_slider->SetValue( num );
}

//-----------------------------------//

} } // end namespaces