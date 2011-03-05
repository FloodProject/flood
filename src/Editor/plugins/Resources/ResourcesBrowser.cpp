/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "PCH.h"
#include "ResourcesBrowser.h"
#include "Viewframe.h"
#include "EditorIcons.h"
#include "UndoManager.h"
#include "Settings.h"
#include "Editor.h"
#include "Utilities.h"
#include "../Scene/ScenePane.h"

#ifdef ALL_PLUGINS

namespace vapor { namespace editor {

//-----------------------------------//

ResourcesBrowser::ResourcesBrowser( EditorFrame* editor, wxWindow* parent,
					wxWindowID id, const wxPoint& pos, const wxSize& size )
	: wxFrame(parent, id, "Resources Browser", pos, wxSize(600, 450))
	, editor(editor)
	, listIndex(0)
{
	setupUI();
	//setupRender();
}

//-----------------------------------//

void ResourcesBrowser::setupRender()
{
	RenderDevice* device = GetEditor().getEngine()->getRenderDevice();

	Settings settings(ThumbSize, ThumbSize);
	renderBuffer = device->createRenderBuffer(settings);
	renderBuffer->createRenderBuffer(RenderBufferType::Depth);
	colorTexture = renderBuffer->createRenderTexture(RenderBufferType::Color);
	
	if( !renderBuffer->check() )
		return;
	
	camera.reset( new Camera() );
	Frustum& frustum = camera->getFrustum();
	frustum.nearPlane = 0.1f;

	entityCamera.reset( new Entity() );
	entityCamera->addTransform();
	entityCamera->addComponent( camera );
	
	scene.reset( new Scene() );
	scene->add( entityCamera );

	renderView = new RenderView(camera);
	renderView->setClearColor(Color::White);
	renderView->setRenderTarget(renderBuffer);
}

//-----------------------------------//
#if 0
void ResourcesBrowser::setupImages()
{
	int size = ThumbSize / 2;

	ResourcesCache::iterator it;
	for( it = resourcesCache.begin(); it != resourcesCache.end(); it++ )
	{
		ResourceMetadata& metadata = it->second;
		
		wxImage image;
		
		if( !image.LoadFile(CacheFolder + metadata.thumbnail) )
			continue;

		image.Rescale(size, size);
		image = image.Mirror(false);

		metadata.index = images->Add( wxBitmap(image) );
		
		std::string base = PathUtils::getBase(metadata.thumbnail);
		m_listCtrl->InsertItem(listIndex++, base, metadata.index);
	}
}
#endif
//-----------------------------------//

void ResourcesBrowser::OnListBeginDrag(wxListEvent& event)
{
	wxTextDataObject data;
	wxDropSource dragSource(this);
	dragSource.SetData(data);
    
	wxDragResult result = dragSource.DoDragDrop( wxDrag_DefaultMove );

	if( result == wxDragCancel || result == wxDragNone )
		return;

	Vector3 dropPoint;

	Vector2i coords = editor->getDropCoords();
	RenderView* view = editor->getMainViewframe()->getView();
	Ray ray = view->getCamera()->getRay(coords.x, coords.y);

	ScenePtr scene = editor->getEngine()->getSceneManager();
	RayTriangleQueryResult res;

	if( scene->doRayTriangleQuery(ray, res) )
	{
		dropPoint = res.intersection;
	}
	else
	{
		Plane ground( Vector3::UnitY, 0 );
		
		float distance;
		if( !ground.intersects(ray, distance) )
			return;
			
		dropPoint = ray.getPoint(distance);
	}

	std::string name = event.GetText();

	ResourceManager* rm = editor->getEngine()->getResourceManager();
	MeshPtr mesh = rm->loadResource<Mesh>(name);

	if( !mesh )
		return;

	EntityPtr entity( new Entity( PathUtils::getFile(name) ) );
	entity->addTransform();
	entity->getTransform()->setPosition(dropPoint);
	entity->addComponent( ModelPtr( new Model(mesh) ) );

	EntityOperation* nodeOperation = new EntityOperation();
	nodeOperation->entity = entity;
	nodeOperation->weakScene = scene;

	UndoManager* undoManager = editor->getUndoManager();
	undoManager->registerOperation(nodeOperation);

	nodeOperation->redo();
}

//-----------------------------------//

void ResourcesBrowser::onConnectClicked(wxCommandEvent& event)
{
	//if( network.createClientSocket("tcp://127.0.0.1:7654") )
	//{
	//	m_button1->SetLabel("Connected");
	//	m_button1->Disable();
	//}

	//std::vector<byte> data;
	//data.push_back( MessageType::ResourceIndexRequest );

	//MessagePtr message = new Message(data);
	////message->setMessageType( MessageType::ResourceIndexRequest );

	//network.sendMessage(message);
}

//-----------------------------------//

void ResourcesBrowser::OnClose(wxCloseEvent& event)
{
    if ( event.CanVeto() )
    {
		// Hide the window instead of closing it.
		Hide();

        event.Veto();
        return;   
    }

    event.Skip();
}

//-----------------------------------//

void ResourcesBrowser::setupUI()
{
	SetWindowStyle(wxDEFAULT_FRAME_STYLE | wxFRAME_TOOL_WINDOW | wxFRAME_FLOAT_ON_PARENT | wxBORDER_NONE );

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	m_panel2 = new wxPanel( this, wxID_ANY );
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );
	
	m_listCtrl = new wxListCtrl( m_panel2, wxID_ANY,
		wxDefaultPosition, wxDefaultSize, wxLC_ICON | wxLC_AUTOARRANGE ); 
	bSizer3->Add( m_listCtrl, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText1 = new wxStaticText( m_panel2, wxID_ANY, "Detail" );
	m_staticText1->Wrap( -1 );
	bSizer5->Add( m_staticText1, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_slider1 = new wxSlider( m_panel2, wxID_ANY, 50, 0, 100 );
	bSizer5->Add( m_slider1, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	bSizer6->Add( bSizer5, 1, wxALIGN_LEFT, 5 );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxHORIZONTAL );
	
	m_button1 = new wxButton( m_panel2, wxID_ANY, "Connect to server" );
	m_button1->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &ResourcesBrowser::onConnectClicked, this );

	bSizer2->Add( m_button1, 1, wxALIGN_RIGHT|wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	bSizer6->Add( bSizer2, 0, wxALIGN_RIGHT, 5 );
	
	bSizer3->Add( bSizer6, 0, wxEXPAND, 5 );
	
	m_panel2->SetSizer( bSizer3 );
	m_panel2->Layout();
	bSizer3->Fit( m_panel2 );
	bSizer1->Add( m_panel2, 1, wxEXPAND, 5 );
	
	SetSizer( bSizer1 );
	Layout();
	
	Centre( wxBOTH );

	Bind( wxEVT_CLOSE_WINDOW, &ResourcesBrowser::OnClose, this );

	wxIcon icon;
	icon.CopyFromBitmap( wxMEMORY_BITMAP(package) );
	
	SetIcon(icon);

	int size = ThumbSize / 2;
	images = new wxImageList(size, size);
	m_listCtrl->AssignImageList(images, wxIMAGE_LIST_NORMAL);

	m_listCtrl->Bind( wxEVT_COMMAND_LIST_BEGIN_DRAG,
		&ResourcesBrowser::OnListBeginDrag, this );
}

//-----------------------------------//

} } // end namespaces

#endif