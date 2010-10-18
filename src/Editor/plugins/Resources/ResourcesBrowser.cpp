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

namespace vapor { namespace editor {

//-----------------------------------//

static const std::string CacheFolder( "Cache/" );
static const std::string ThumbCache( "Thumbs.cache" );
static const int ThumbSize = 256;

//-----------------------------------//

ResourcesBrowser::ResourcesBrowser( EditorFrame* editor,
							wxWindow* parent, wxWindowID id,
							const wxPoint& pos, const wxSize& size )
	: wxFrame(parent, id, "Resources Browser", pos, wxSize(200, 500),
		wxDEFAULT_FRAME_STYLE | /*wxFRAME_TOOL_WINDOW |
		wxFRAME_FLOAT_ON_PARENT |*/ wxBORDER_NONE, "ResourcesBrowser")
	, editor(editor)
	, listIndex(0)
{
	setupUI();
	setupRender();
	loadCache();
}

//-----------------------------------//

ResourcesBrowser::~ResourcesBrowser()
{
	saveCache();
}

//-----------------------------------//

void ResourcesBrowser::setupUI()
{
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
	
	m_button1 = new wxButton( m_panel2, wxID_ANY, "Refresh" );
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

void ResourcesBrowser::setupRender()
{
	Engine* engine = Engine::getInstancePtr();
	RenderDevice* device = engine->getRenderDevice();

	Settings settings(ThumbSize, ThumbSize);
	renderBuffer = device->createRenderBuffer(settings);
	renderTexture = renderBuffer->createRenderTexture();
	
	camera.reset( new Camera() );
	camera->getFrustum().projectionType = Projection::Perspective;
	
	nodeCamera.reset( new Node() );
	nodeCamera->addTransform();
	nodeCamera->addComponent( camera );
	
	scene.reset( new Scene() );
	scene->add( nodeCamera );

	renderView = new View(camera);
	renderView->setClearColor(Color::White);
	renderView->setRenderTarget(renderBuffer);
}

//-----------------------------------//

static const std::string getBase(const std::string& name)
{
	// Check if it has a file extension.
	size_t ch = name.find_last_of(".");

	if( ch == std::string::npos ) 
		return "";

	// Return the file extension.
	return name.substr( 0, ch );
}

void ResourcesBrowser::setupImages()
{
	int size = ThumbSize / 2;

	foreach( ResourcesCachePair p, resourcesCache )
	{
		ResourceMetadata& metadata = p.second;
		
		wxImage image;
		if( !image.LoadFile(CacheFolder + metadata.thumbnail) )
			continue;

		image.Rescale(size, size);
		image = image.Mirror(false);

		metadata.index = images->Add( wxBitmap(image) );
		m_listCtrl->InsertItem(listIndex++, getBase(metadata.thumbnail), metadata.index);
	}
}

//-----------------------------------//

bool ResourcesBrowser::loadCache()
{
	LocaleSaveRestore locale;

	NativeFile file( CacheFolder + ThumbCache, FileMode::Read );

	if( !file.open() )
		return false;

	std::string str = file.readString();

	Json::Value root;
	Json::Reader jsonReader;
	bool success = jsonReader.parse(str, root, false);

	if( !success )
		return false;

	for( uint i = 0; i < root.size(); i++ )
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

	return true;
}

//-----------------------------------//

bool ResourcesBrowser::saveCache()
{
	LocaleSaveRestore locale;

	NativeFile file( CacheFolder + ThumbCache, FileMode::Write );

	if( !file.open() )
		return false;
	
	Json::Value root;

	static uint i = 0;

	foreach( const ResourcesCachePair& p, resourcesCache )
	{
		const ResourceMetadata& metadata = p.second;
		
		Json::Value value;
		value["hash"] = metadata.hash;
		value["thumb"] = metadata.thumbnail;
		
		root[i++] = value;
	}

	file.write( root.toStyledString() );

	return true;
}

//-----------------------------------//

void ResourcesBrowser::scanFiles()
{
	Engine* engine = Engine::getInstancePtr();
	ResourceManager* rm = engine->getResourceManager();
	FileSystem* fs = engine->getFileSystem();

	std::vector<std::string> files = fs->enumerateFiles("meshes");

	wxProgressDialog progressDialog( "Resources loading progress",
		"Please wait while resources are loaded.", files.size(), this,
		wxPD_AUTO_HIDE | wxPD_SMOOTH | wxPD_CAN_ABORT );
	
	progressDialog.Show();

	uint progress = 0;

	foreach( const std::string& path, files )
	{
		progressDialog.Update(progress++, path);

		if( progressDialog.WasCancelled() )
			break;

		File file(path);
		uint hash = Hash::Murmur2( file.read(), 0xBEEF );
		
		if( resourcesCache.find(hash) != resourcesCache.end() )
			continue;

		MeshPtr mesh = rm->loadResource<Mesh>(path, false);

		if( !mesh || mesh->getResourceGroup() != ResourceGroup::Meshes )
			continue;

		ResourceMetadata metadata;
		metadata.hash = hash;
		metadata.thumbnail = mesh->getPath() + ".png";
		metadata.path = mesh->getPath();
		resourcesCache[hash] = metadata;

		ImagePtr thumb = generateThumbnail(mesh);

		if( !thumb )
			continue;

		thumb->save( CacheFolder + metadata.thumbnail );
	}
}

//-----------------------------------//

ImagePtr ResourcesBrowser::generateThumbnail(const MeshPtr& mesh)
{
	NodePtr nodeResource( new Node() );
	nodeResource->addTransform();
	nodeResource->addComponent( ModelPtr(new Model(mesh)) );
	scene->add( nodeResource );

	TransformPtr transResource = nodeResource->getTransform();
	const BoundingBox& box = mesh->getBoundingVolume();
	const Vector3& center = box.getCenter();
	transResource->setPosition( Vector3(-center.x, -center.y, 0) );
	//transResource->setScale( Vector3(-1, -1, 0) );

	TransformPtr transCamera = nodeCamera->getTransform();
	const Frustum& frustum = camera->getFrustum();
	
	float distance = ((box.max.y - box.min.y) / 2)
		/ std::tan(Math::degreeToRadian(frustum.fieldOfView) / 2);
	
	transCamera->setPosition(
		Vector3(0, 0, -distance - ((box.max.z - box.min.z) / 2)) );

	scene->update(0);

	renderBuffer->bind();
	renderView->update();
	ImagePtr image = renderTexture->readImage();
	renderBuffer->unbind();

	scene->remove( nodeResource );

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

	Vector2 coords = editor->getDropCoords();
	View* view = editor->getMainViewframe()->getView();
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

	NodePtr node( new Node("Node") );
	node->addTransform();
	node->getTransform()->setPosition(dropPoint);
	node->addComponent( ModelPtr( new Model(mesh) ) );
	scene->add(node);
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

} } // end namespaces