/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Editor/API.h"

#ifdef ENABLE_PLUGIN_TERRAIN

#include "TerrainPage.h"
#include "Core/Utilities.h"
#include "Resources/ResourceManager.h"
#include "Settings.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

TerrainPage::TerrainPage( wxWindow* parent, wxWindowID id,
						 const wxPoint& pos, const wxSize& size, long style ) 
	: wxScrolledWindow( parent, id, pos, size, style )
{	
	createUI();
	createBrushes();
}

//-----------------------------------//

void TerrainPage::createBrushes()
{
	String brushesPath = PathCombine(MediaFolder, "Brushes");
	
	std::vector<String> files;
	FileEnumerateFiles(brushesPath, files);

	ResourceManager* rm = GetResourceManager();

	for( size_t i = 0; i < files.size(); i++ )
	{
		const Path& path = files[i];
		
		ImageHandle handle = rm->loadResource<Image>(path);

		Image* image = handle.Resolve();
		if( !image ) continue;

		while( !image->isLoaded() )
			SystemSleep(0.01f);

		Path fullPath = PathCombine(brushesPath, path);

		wxBitmap* bmp = ConvertImageToBitmap(image, fullPath);
		if( !bmp ) continue;

		m_choiceBrush->Append(PathGetFile(path), *bmp);
		delete bmp;
	}

	if( m_choiceBrush->GetCount() > 0 ) m_choiceBrush->Select(0);
}

//-----------------------------------//

void TerrainPage::createUI()
{
	createBrush();
}

//-----------------------------------//

void TerrainPage::createBrush()
{
	m_panelBrush = this;

	wxFlexGridSizer* fgSizer2 = new wxFlexGridSizer(2);
	fgSizer2->AddGrowableCol(1);

	wxSizerFlags flags = wxSizerFlags().Expand().Border();

	wxStaticText* m_staticText2 = new wxStaticText( m_panelBrush, wxID_ANY, "Brush:" );
	fgSizer2->Add( m_staticText2, flags );
	
	m_choiceBrush = new wxBitmapComboBox( m_panelBrush, wxID_ANY );
	fgSizer2->Add( m_choiceBrush, flags );
	
	m_choiceBrush->Bind( wxEVT_COMMAND_COMBOBOX_SELECTED, &TerrainPage::onTextureSelected, this );

	wxStaticText* m_staticText31 = new wxStaticText( m_panelBrush, wxID_ANY, "Size:" );
	fgSizer2->Add( m_staticText31, flags );
	
	m_sliderSize = new wxSliderCtrl( m_panelBrush, wxID_ANY, BRUSH_INITIAL_SIZE, 0, 200 );
	fgSizer2->Add( m_sliderSize, flags );
	
	wxStaticText* m_staticText3 = new wxStaticText( m_panelBrush, wxID_ANY, "Strength:" );
	fgSizer2->Add( m_staticText3, flags );

	m_sliderStrength = new wxSliderCtrl( m_panelBrush, wxID_ANY, 2, 0, 100 );
	fgSizer2->Add( m_sliderStrength, flags );

	wxStaticText* m_staticText4 = new wxStaticText( m_panelBrush, wxID_ANY, "Texture:" );
	fgSizer2->Add( m_staticText4, flags );

	m_textureChoice = new wxComboBox( m_panelBrush, wxID_ANY );
	fgSizer2->Add( m_textureChoice, flags );

	m_textureChoice->Bind( wxEVT_COMMAND_COMBOBOX_DROPDOWN, &TerrainPage::onTextureDropdown, this );
	m_textureChoice->Bind( wxEVT_COMMAND_COMBOBOX_SELECTED, &TerrainPage::onTextureSelected, this );

	wxStaticText* m_staticText5 = new wxStaticText( m_panelBrush, wxID_ANY, "Tile:" );
	fgSizer2->Add( m_staticText5, flags );

	wxBoxSizer* tileSizer = new wxBoxSizer( wxVERTICAL );
	
	m_tileLock = new wxCheckBox( m_panelBrush, wxID_ANY, "Lock" );
	m_tileLock->SetValue(false);
	tileSizer->Add( m_tileLock, flags );

	m_tileOffsetX = new wxSliderCtrl( m_panelBrush, wxID_ANY, 0, 0, 100 );
	tileSizer->Add( m_tileOffsetX, flags );

	m_tileOffsetY = new wxSliderCtrl( m_panelBrush, wxID_ANY, 0, 0, 100 );
	tileSizer->Add( m_tileOffsetY, flags );

	fgSizer2->Add( tileSizer, flags );

	SetSizerAndFit( fgSizer2 );
}

//-----------------------------------//

void TerrainPage::createCell()
{
}

//-----------------------------------//

void TerrainPage::onBrushSelected(wxCommandEvent& event)
{
	String brush = event.GetString();
	brushHandle = GetResourceManager()->loadResource<Image>(brush);
}

//-----------------------------------//

void TerrainPage::onBrushDropdown(wxCommandEvent& event)
{

}

//-----------------------------------//

void TerrainPage::onTextureSelected(wxCommandEvent& event)
{

}

//-----------------------------------//

void TerrainPage::onTextureDropdown(wxCommandEvent& event)
{
	m_textureChoice->Clear();

	ResourceManager* rm = GetResourceManager();
	const ResourceMap& resources = rm->getResources();

	ResourceMap::const_iterator it;
	for( it = resources.begin(); it != resources.end(); it++ )
	{
		const ResourceHandle& handle = it->second;
		
		Resource* resource = handle.Resolve();
		if( ! resource ) continue;
		
		if( resource->getResourceGroup() != ResourceGroup::Images )
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

ImageHandle TerrainPage::getBrushImage()
{
	Path path = m_choiceBrush->GetValue();

	ResourceManager* rm = GetResourceManager();
	return rm->loadResource<Image>(path);
}

//-----------------------------------//

ImagePtr TerrainPage::getPaintImage()
{
	Path path = m_textureChoice->GetValue();

	ResourceManager* rm = GetResourceManager();
	return rm->loadResource<Image>(path).Resolve();
}

//-----------------------------------//

NAMESPACE_EDITOR_END

#endif