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

class EditorFrame;

/**
 * A resources browser is a window where you can choose a resource from
 * the existing resouces of a given type.
 */

class ResourcesBrowser : public wxFrame 
{
public:

	ResourcesBrowser( EditorFrame* editor,
		wxWindow* parent, wxWindowID id = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize );

	// Sets up the images.
	void setupImages();

	// Generate thumbnails.
	void generateThumbnails(const std::vector<std::string>& files);

	// Generate thumbnail.
	ImagePtr generateThumbnail(const MeshPtr& mesh);

protected:

	// Sets up the UI.
	void setupUI();

	// Sets up the render buffers.
	void setupRender();

	// Event handlers.
	void OnClose(wxCloseEvent& event);
	void OnListBeginDrag(wxListEvent& event);
	void onConnectClicked(wxCommandEvent& event);

	EditorFrame* editor;
	wxImageList* images;
	wxPanel* m_panel2;
	wxListCtrl* m_listCtrl;
	wxStaticText* m_staticText1;
	wxSlider* m_slider1;
	wxButton* m_button1;
	int listIndex;

	ScenePtr scene;
	EntityPtr entityCamera;
	CameraPtr camera;
	RenderView* renderView;

	TexturePtr depthTexture;
	TexturePtr colorTexture;
	RenderBuffer* renderBuffer;
};

//-----------------------------------//

} } // end namespaces