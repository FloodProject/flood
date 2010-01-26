/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "PCH.h"
#include "TerrainPage.h"

namespace vapor { namespace editor {

////////////////////////////////////////////////////////////
// Event table
////////////////////////////////////////////////////////////
BEGIN_EVENT_TABLE(TerrainPage, wxPanel)
END_EVENT_TABLE()

//-----------------------------------//

TerrainPage::TerrainPage( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	m_cbTerrainTool = new wxChoicebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxCHB_DEFAULT );
	m_panelBrush = new wxPanel( m_cbTerrainTool, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxVERTICAL );
	
	m_staticline5 = new wxStaticLine( m_panelBrush, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer11->Add( m_staticline5, 0, wxEXPAND|wxTOP|wxBOTTOM, 5 );
	
	wxFlexGridSizer* fgSizer2;
	fgSizer2 = new wxFlexGridSizer( 3, 2, 0, 0 );
	fgSizer2->AddGrowableCol( 1 );
	fgSizer2->SetFlexibleDirection( wxHORIZONTAL );
	fgSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText2 = new wxStaticText( m_panelBrush, wxID_ANY, wxT("Brush:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	fgSizer2->Add( m_staticText2, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxArrayString m_choiceBrushChoices;
	m_choiceBrush = new wxChoice( m_panelBrush, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceBrushChoices, 0 );
	m_choiceBrush->SetSelection( 0 );
	fgSizer2->Add( m_choiceBrush, 1, wxEXPAND|wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxLEFT, 5 );
	
	m_staticText31 = new wxStaticText( m_panelBrush, wxID_ANY, wxT("Size:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText31->Wrap( -1 );
	fgSizer2->Add( m_staticText31, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	wxBoxSizer* bSizer81;
	bSizer81 = new wxBoxSizer( wxHORIZONTAL );
	
	m_sliderSize = new wxSlider( m_panelBrush, wxID_ANY, 1, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	bSizer81->Add( m_sliderSize, 1, wxALL|wxEXPAND, 5 );
	
	m_textCtrlSize = new wxTextCtrl( m_panelBrush, wxID_ANY, wxT("1"), wxDefaultPosition, wxSize( 30,-1 ), 0|wxDOUBLE_BORDER );
	bSizer81->Add( m_textCtrlSize, 0, wxTOP|wxBOTTOM, 5 );
	
	fgSizer2->Add( bSizer81, 1, wxEXPAND, 5 );
	
	m_staticText3 = new wxStaticText( m_panelBrush, wxID_ANY, wxT("Strength:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	fgSizer2->Add( m_staticText3, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxHORIZONTAL );
	
	m_sliderStrength = new wxSlider( m_panelBrush, wxID_ANY, 1, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	bSizer8->Add( m_sliderStrength, 1, wxALL|wxEXPAND, 5 );
	
	m_textCtrlStrength = new wxTextCtrl( m_panelBrush, wxID_ANY, wxT("1"), wxDefaultPosition, wxSize( 30,-1 ), 0|wxDOUBLE_BORDER );
	bSizer8->Add( m_textCtrlStrength, 0, wxTOP|wxBOTTOM, 5 );
	
	fgSizer2->Add( bSizer8, 1, wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );
	
	bSizer11->Add( fgSizer2, 1, wxEXPAND, 5 );
	
	m_panelBrush->SetSizer( bSizer11 );
	m_panelBrush->Layout();
	bSizer11->Fit( m_panelBrush );
	m_cbTerrainTool->AddPage( m_panelBrush, wxT("Raise or Lower"), true );
	bSizer1->Add( m_cbTerrainTool, 1, wxEXPAND|wxALL, 5 );
	
	this->SetSizer( bSizer1 );
	this->Layout();
}

//-----------------------------------//

} } // end namespaces