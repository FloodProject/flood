/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "PCH.h"
#include "TerrainPage.h"

namespace vapor { namespace editor {

//-----------------------------------//

BEGIN_EVENT_TABLE(TerrainPage, wxPanel)
	//
END_EVENT_TABLE()

//-----------------------------------//

TerrainPage::TerrainPage( Engine* engine, wxWindow* parent, wxWindowID id, 
						 const wxPoint& pos, const wxSize& size,
						 long style ) 
	: wxPanel( parent, id, pos, size, style )
{	
	createUI();

	ResourceManager* rm = engine->getResourceManager();
	const ImagePtr& image = 
		rm->loadResource<Image>("brushes/default.png", false);
	
	if(image)
		m_choiceBrush->addImage( "Brush01", image );
}

//-----------------------------------//

void TerrainPage::createBrush()
{
	m_panelBrush = new wxPanel( m_cbTerrainTool, wxID_ANY, 
		wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );

	new wxStaticLine( m_panelBrush, wxID_ANY, 
		wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );

	wxFlexGridSizer* fgSizer2;
	fgSizer2 = new wxFlexGridSizer( 2 );
	fgSizer2->SetFlexibleDirection( wxBOTH );
	fgSizer2->AddGrowableCol( 1 );
	
	wxStaticText* m_staticText2 = new wxStaticText( m_panelBrush, wxID_ANY, 
		wxT("Brush:"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer2->Add( m_staticText2, 0, wxALL, 5 );
	
	m_choiceBrush = new wxImageComboBox( m_panelBrush, wxID_ANY );
	fgSizer2->Add( m_choiceBrush, 0, wxEXPAND|wxTOP|wxBOTTOM|wxLEFT, 5 );
	
	wxStaticText* m_staticText31 = new wxStaticText( m_panelBrush, wxID_ANY, 
		wxT("Size:"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer2->Add( m_staticText31, 0, wxALL, 5 );
	
	m_sliderSize = new wxSliderCtrl( m_panelBrush, wxID_ANY, 1, 0, 100 );
	fgSizer2->Add( m_sliderSize, 0, wxEXPAND|wxTOP|wxBOTTOM, 5 );
	
	wxStaticText* m_staticText3 = new wxStaticText( m_panelBrush, wxID_ANY, 
		wxT("Strength:"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer2->Add( m_staticText3, 0, wxALL, 5 );

	m_sliderStrength = new wxSliderCtrl( m_panelBrush, wxID_ANY, 1, 0, 100 );
	fgSizer2->Add( m_sliderStrength, 0, wxEXPAND|wxTOP|wxBOTTOM, 5 );

	m_panelBrush->SetSizer( fgSizer2 );
	m_panelBrush->Layout();

	m_cbTerrainTool->AddPage( m_panelBrush, "Brush Settings", true );
}

//-----------------------------------//

void TerrainPage::createCell()
{
	m_panelCell = new wxPanel( m_cbTerrainTool, wxID_ANY, 
		wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );

	m_cbTerrainTool->AddPage( m_panelCell, "Cell Placement", true );
}

//-----------------------------------//

void TerrainPage::createUI()
{
	m_cbTerrainTool = new wxChoicebook( this, wxID_ANY, 
		wxDefaultPosition, wxDefaultSize, wxCHB_DEFAULT );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	bSizer1->Add( m_cbTerrainTool, 1, wxEXPAND|wxALL, 5 );
	
	createBrush();
	//createCell();

	SetSizer( bSizer1 );
	Layout();
}

//-----------------------------------//

} } // end namespaces