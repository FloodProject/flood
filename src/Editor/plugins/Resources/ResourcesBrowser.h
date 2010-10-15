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

struct ResourceMetadata
{
	// Hash of the resource.
	uint hash;

	// Thumbnail of the resource.
	std::string thumbnail;
};

typedef std::map<uint, ResourceMetadata> ResourcesCache;
typedef std::pair<uint, ResourceMetadata> ResourcesCachePair;

//-----------------------------------//

/**
 * A resources browser is a window where you can choose a resource from
 * the existing resouces of a given type.
 */

class ResourcesBrowser : public wxFrame 
{
public:

	ResourcesBrowser( wxWindow* parent, wxWindowID id = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize );

	~ResourcesBrowser();

	// Scans for known resources.
	void scanFiles();

	// Loads the thumbnails cache.
	bool loadCache();

	// Saves the thumbnails cache.
	bool saveCache();

	// Generate thumbnail.
	ImagePtr generateThumbnail(const MeshPtr& mesh);

protected:

	// Sets up the UI.
	void setupUI();

	// Sets up the render buffers.
	void setupRender();

	// Event handlers.
	void OnClose(wxCloseEvent& event);

	wxPanel* m_panel2;
	wxListBox* m_listBox1;
	wxStaticText* m_staticText1;
	wxSlider* m_slider1;
	wxButton* m_button1;

	ScenePtr scene;
	NodePtr nodeCamera;
	CameraPtr camera;
	View* renderView;
	TexturePtr renderTexture;
	RenderBuffer* renderBuffer;

	// Caches all the resources metadata.
	ResourcesCache resourcesCache;
};

//-----------------------------------//

} } // end namespaces