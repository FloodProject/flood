/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Editor/API.h"
#include "TerrainPage.h"
#include "Utilities.h"
#include "Resources\ResourceManager.h"
#include <iterator>

namespace vapor { namespace editor {

//-----------------------------------//

TerrainPage::TerrainPage( wxWindow* parent, wxWindowID id,
						 const wxPoint& pos, const wxSize& size, long style ) 
	: wxPanel( parent, id, pos, size, style )
{	
	createUI();
	createBrushes();
}

//-----------------------------------//

void TerrainPage::createBrushes()
{
	String brushesPath = "Media/Brushes/";
	
	std::vector<String> files;
	FileEnumerateFiles(brushesPath, files);

	ResourceManager* rm = GetResourceManager();

	for( uint i = 0; i < files.size(); i++ )
	{
		const std::string& file = files[i];

		ImagePtr image = rm->loadResource<Image>(file);
		
		if( !image )
			continue;

		m_choiceBrush->addImage( image );
	}
}

//-----------------------------------//

void TerrainPage::createUI()
{
	m_cbTerrainTool = new wxChoicebook( this, wxID_ANY, 
		wxDefaultPosition, wxDefaultSize, wxCHB_DEFAULT );

	wxBoxSizer* bSizer1 = new wxBoxSizer( wxVERTICAL );
	bSizer1->Add( m_cbTerrainTool, 1, wxEXPAND|wxALL, 5 );
	
	createBrush();
	//createCell();

	SetSizer( bSizer1 );
}

//-----------------------------------//

void TerrainPage::createBrush()
{
	m_panelBrush = new wxPanel( m_cbTerrainTool );

	//new wxStaticLine( m_panelBrush, wxID_ANY, 
	//	wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );

	wxFlexGridSizer* fgSizer2 = new wxFlexGridSizer( 2 );
	fgSizer2->SetFlexibleDirection( wxBOTH );
	fgSizer2->AddGrowableCol( 1 );
	
	wxStaticText* m_staticText2 = new wxStaticText( m_panelBrush, wxID_ANY, "Brush:" );
	fgSizer2->Add( m_staticText2, 0, wxALL, 5 );
	
	m_choiceBrush = new wxImageComboBox( m_panelBrush, wxID_ANY );
	fgSizer2->Add( m_choiceBrush, 0, wxEXPAND|wxTOP|wxBOTTOM|wxLEFT, 5 );
	
	wxStaticText* m_staticText31 = new wxStaticText( m_panelBrush, wxID_ANY, "Size:" );
	fgSizer2->Add( m_staticText31, 0, wxALL, 5 );
	
	m_sliderSize = new wxSliderCtrl( m_panelBrush, wxID_ANY, 50, 0, 100 );
	fgSizer2->Add( m_sliderSize, 0, wxEXPAND|wxTOP|wxBOTTOM, 5 );
	
	wxStaticText* m_staticText3 = new wxStaticText( m_panelBrush, wxID_ANY,	"Strength:" );
	fgSizer2->Add( m_staticText3, 0, wxALL, 5 );

	m_sliderStrength = new wxSliderCtrl( m_panelBrush, wxID_ANY, 2, 0, 100 );
	fgSizer2->Add( m_sliderStrength, 0, wxEXPAND|wxTOP|wxBOTTOM, 5 );

	wxStaticText* m_staticText4 = new wxStaticText( m_panelBrush, wxID_ANY, "Texture:" );
	fgSizer2->Add( m_staticText4, 0, wxALL, 5 );

	m_textureChoice = new wxComboBox( m_panelBrush, wxID_ANY );
	fgSizer2->Add( m_textureChoice, 0, wxEXPAND|wxTOP|wxBOTTOM|wxLEFT, 5 );

	m_textureChoice->Bind( wxEVT_COMMAND_COMBOBOX_DROPDOWN,
		&TerrainPage::onComboBoxDropdown, this );

	m_textureChoice->Bind( wxEVT_COMMAND_COMBOBOX_SELECTED,
		&TerrainPage::onComboBoxSelected, this );

	wxStaticText* m_staticText5 = new wxStaticText( m_panelBrush, wxID_ANY, "Tile:" );
	fgSizer2->Add( m_staticText5, 0, wxALL, 5 );

	wxBoxSizer* tileSizer = new wxBoxSizer( wxVERTICAL );
	
	m_tileLock = new wxCheckBox( m_panelBrush, wxID_ANY, "Lock" );
	m_tileLock->SetValue(false);
	tileSizer->Add( m_tileLock, 0, wxEXPAND|wxTOP|wxBOTTOM|wxLEFT, 5 );

	m_tileOffsetX = new wxSliderCtrl( m_panelBrush, wxID_ANY, 0, 0, 100 );
	tileSizer->Add( m_tileOffsetX, 0, wxEXPAND|wxTOP|wxBOTTOM, 5 );

	m_tileOffsetY = new wxSliderCtrl( m_panelBrush, wxID_ANY, 0, 0, 100 );
	tileSizer->Add( m_tileOffsetY, 0, wxEXPAND|wxTOP|wxBOTTOM, 5 );

	fgSizer2->Add( tileSizer, 0, wxEXPAND|wxTOP|wxBOTTOM, 5 );

	m_panelBrush->SetSizer( fgSizer2 );
	m_panelBrush->Layout();

	m_cbTerrainTool->AddPage( m_panelBrush, "Brush Settings", true );
}

//-----------------------------------//

void TerrainPage::createCell()
{
	m_panelCell = new wxPanel( m_cbTerrainTool, wxID_ANY, 
		wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );

	m_cbTerrainTool->AddPage( m_panelCell, "Cell Settings", true );
}

//-----------------------------------//

void TerrainPage::onComboBoxSelected(wxCommandEvent& event)
{
}

//-----------------------------------//

void TerrainPage::onComboBoxDropdown(wxCommandEvent& event)
{
	m_textureChoice->Clear();

	ResourceManager* rm = GetResourceManager();
	const ResourceMap& resources = rm->getResources();

	ResourceMap::const_iterator it;
	for( it = resources.begin(); it != resources.end(); it++ )
	{
		if( it->second->getResourceGroup() != ResourceGroup::Images )
			continue;

		m_textureChoice->Append(it->first);
	}
}

//-----------------------------------//

int	TerrainPage::getBrushStrength()
{
	return m_sliderStrength->GetSlider()->GetValue();
}

//-----------------------------------//

int	TerrainPage::getBrushSize()
{
	return m_sliderSize->GetSlider()->GetValue();
}

//-----------------------------------//

ImagePtr TerrainPage::getPaintImage()
{
	std::string path = m_textureChoice->GetValue();

	ResourceManager* rm = GetResourceManager();
	return rm->loadResource<Image>(path);
}

//-----------------------------------//

} } // end namespaces