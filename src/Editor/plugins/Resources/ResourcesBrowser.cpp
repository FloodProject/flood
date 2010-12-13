/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "PCH.h"
#include "ResourcesBrowser.h"
#include "Editor.h"
#include "Viewframe.h"
#include "EditorIcons.h"
#include "Settings.h"
#include "UndoManager.h"
#include "../Scene/ScenePage.h"
#include "Utilities.h"

namespace vapor { namespace editor {

//-----------------------------------//

ResourcesBrowser::ResourcesBrowser( EditorFrame* editor, wxWindow* parent,
					wxWindowID id, const wxPoint& pos, const wxSize& size )
	: wxFrame(parent, id, "Resources Browser", pos, wxSize(600, 450))
	, editor(editor)
	, listIndex(0)
{
	setupUI();
	setupRender();
	loadCache();
	//network.init();
}

//-----------------------------------//

ResourcesBrowser::~ResourcesBrowser()
{
	saveCache();
}

//-----------------------------------//

void ResourcesBrowser::setupRender()
{
	Engine* engine = Engine::getInstancePtr();
	RenderDevice* device = engine->getRenderDevice();

	Settings settings(ThumbSize, ThumbSize);
	renderBuffer = device->createRenderBuffer(settings);
	renderBuffer->createRenderBuffer(RenderBufferType::Depth);
	colorTexture = renderBuffer->createRenderTexture(RenderBufferType::Color);
	
	if( !renderBuffer->check() )
		return;
	
	camera.reset( new Camera() );
	Frustum& frustum = camera->getFrustum();
	frustum.nearPlane = 0.1f;

	nodeCamera.reset( new Entity() );
	nodeCamera->addTransform();
	nodeCamera->addComponent( camera );
	
	scene.reset( new Scene() );
	scene->add( nodeCamera );

	renderView = new RenderView(camera);
	renderView->setClearColor(Color::White);
	renderView->setRenderTarget(renderBuffer);
}

//-----------------------------------//

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
		
		std::string base = Path::getBase(metadata.thumbnail);
		m_listCtrl->InsertItem(listIndex++, base, metadata.index);
	}
}

//-----------------------------------//

bool ResourcesBrowser::loadCache()
{
	LocaleSwitch locale;
	std::string path = CacheFolder + ThumbCache;

	//if( !File::exists(path) )
	//{
	//	Log::warn("Could not find thumbnails cache file '%s'", path.c_str());
	//	return false;
	//}

	FileStream file( path, StreamMode::Read );

	if( !file.open() )
	{
		Log::warn("Could not open thumbnails cache file '%s'", path.c_str());
		return false;
	}

	std::string text;
	file.read(text);

	Json::Value root;
	Json::Reader reader;
	
	if( !reader.parse(text, root, false) )
		return false;

	uint i = 0;
	while( i++ < root.size() )
	{
		if( !root.isValidIndex(i) )
			continue;

		const Json::Value& value = root[i];

		if( value.isNull() || value.empty() )
			continue;

		if( !value.isMember("hash") || !value.isMember("thumb") )
			continue;

		const Json::Value& valHash = value["hash"];
		const Json::Value& valThumb = value["thumb"];

		if( valHash.isNull() || valHash.empty() )
			continue;

		if( valThumb.isNull() || valThumb.empty() )
			continue;

		ResourceMetadata metadata;
		metadata.hash = valHash.asUInt();
		metadata.thumbnail = valThumb.asString();

		resourcesCache[metadata.hash] = metadata;
	}

	Log::info("Loaded thumbnails cache from '%s' with %u entries", path.c_str(), i);

	return true;
}

//-----------------------------------//

bool ResourcesBrowser::saveCache()
{
	LocaleSwitch locale;
	std::string path = CacheFolder + ThumbCache;
	FileStream file( path, StreamMode::Write );

	if( !file.open() )
		return false;
	
	Json::Value root;
	uint i = 0;

	ResourcesCache::const_iterator it;
	for( it = resourcesCache.cbegin(); it != resourcesCache.cend(); it++ )
	{
		const ResourceMetadata& metadata = it->second;
		
		Json::Value value;
		value["hash"] = metadata.hash;
		value["thumb"] = metadata.thumbnail;
		
		root[i++] = value;
	}

	#pragma TODO(Write)
	//file.write( root.toStyledString() );
	Log::info("Wrote thumbnails cache to '%s' with %u entries", path.c_str(), i);

	return true;
}

//-----------------------------------//

void ResourcesBrowser::scanFiles()
{
	Engine* engine = editor->getEngine();
	ResourceManager* rm = engine->getResourceManager();
	
	bool threadedStatus = rm->getThreadedLoading();
	rm->setThreadedLoading(false);

	std::vector<std::string> files;
	
	std::vector<std::string> found = System::enumerateFiles("media/meshes");
	
	for( uint i = 0; i < found.size(); i++ )
	{
		const std::string& path = found[i];

		std::string ext = Path::getExtension(path);
		ResourceLoader* loader = rm->findLoader(ext);

		if( !loader )
			continue;

		if( loader->getResourceGroup() != ResourceGroup::Meshes )
			continue;

		files.push_back(path);
	}

	wxProgressDialog progressDialog( "Loading resources",
		"Please wait while resources are loaded.", files.size(),
		this, wxPD_AUTO_HIDE | wxPD_SMOOTH | wxPD_CAN_ABORT );
	
	progressDialog.Show();

	uint progress = 0;

	for( uint i = 0; i < files.size(); i++ )
	{
		const std::string& path = files[i];
		
		// Force unused resources to be unloaded.
		rm->update(0);

		progressDialog.Update(progress++, Path::getFile(path));

		if( progressDialog.WasCancelled() )
			break;

		File file(path);
		
		std::vector<byte> data;
		file.read(data);

		uint hash = Hash::Murmur2( data, 0xBEEF );
		
		if( resourcesCache.find(hash) != resourcesCache.end() )
			continue;

		MeshPtr mesh = rm->loadResource<Mesh>(path, false);

		if( !mesh || mesh->getResourceGroup() != ResourceGroup::Meshes )
			continue;

		const std::string& resPath = Path::getFile(mesh->getPath());

		ResourceMetadata metadata;
		metadata.hash = hash;
		metadata.thumbnail = resPath + ".png";
		metadata.path = resPath;
		resourcesCache[hash] = metadata;

		ImagePtr thumb = generateThumbnail(mesh);

		if( !thumb )
			continue;

		thumb->save( CacheFolder + metadata.thumbnail );

		Log::info("Generated thumbnail for resource '%s'", resPath.c_str());
	}

	rm->setThreadedLoading(threadedStatus);
}

//-----------------------------------//

ImagePtr ResourcesBrowser::generateThumbnail(const MeshPtr& mesh)
{
	EntityPtr nodeResource( new Entity() );
	nodeResource->addTransform();
	nodeResource->addComponent( ModelPtr(new Model(mesh)) );
	scene->add( nodeResource );

	const BoundingBox& box = mesh->getBoundingVolume();
	const Vector3& center = box.getCenter();

	TransformPtr transResource = nodeResource->getTransform();
	transResource->setPosition( Vector3(-center.x, -center.y, 0) );

	Vector3 size = (box.max - box.min) / 2;
	float maxSize = std::max(size.x, std::max(size.y, size.z));
	
	const Frustum& frustum = camera->getFrustum();
	float fovRad = Math::degreeToRadian(frustum.fieldOfView);
	float distance = maxSize / std::tan(fovRad / 2) + size.z;
	
	TransformPtr transCamera = nodeCamera->getTransform();
	transCamera->setPosition(Vector3(0, 0, -distance));

	scene->update(0);

	renderBuffer->bind();
	renderView->update();
	renderBuffer->unbind();

	scene->remove( nodeResource );

	ImagePtr image = colorTexture->readImage();

	return image;
}

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

	EntityPtr node( new Entity( Path::getFile(name) ) );
	node->addTransform();
	node->getTransform()->setPosition(dropPoint);
	node->addComponent( ModelPtr( new Model(mesh) ) );

	EntityOperation* nodeOperation = new EntityOperation();
	nodeOperation->node = node;
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