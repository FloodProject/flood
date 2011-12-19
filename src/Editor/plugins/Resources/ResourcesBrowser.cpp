/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Editor/API.h"
#include "ResourcesBrowser.h"
#include "DocumentWindow.h"

#include "Editor.h"
#include "EditorIcons.h"
#include "Settings.h"

#include "Core/Utilities.h"
#include "Core/PluginManager.h"

#include "EventManager.h"
#include "UndoManager.h"

#include "ResourcesPlugin.h"
#include "Resources/ResourceDatabase.h"

#include "Plugins/Scene/ScenePane.h"
#include "Plugins/Scene/SceneDocument.h"

#include "Plugins/Property/PropertyPlugin.h"
#include "Plugins/Property/PropertyPage.h"

#ifdef ENABLE_RESOURCE_BROWSER

#define BROWSER_DEFAULT_STYLE (wxDEFAULT_DIALOG_STYLE & wxRESIZE_BORDER)

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

ResourcesBrowser::ResourcesBrowser( wxWindow* parent )
	: ResourcesFrame(parent, wxID_ANY, "Resources Browser")
	, m_resourceImages(nullptr)
	, m_resourceGroupsImages(nullptr)
	, inSelectionMode(false)
	, database(nullptr)
{
	setupUI();
	setupGroupIcons();
	setupGroups();
}

//-----------------------------------//

ResourcesBrowser::~ResourcesBrowser()
{

}

//-----------------------------------//

void ResourcesBrowser::setDatabase( ResourceDatabase* db )
{
	if( !db && database )
	{
		database->onResourceAdded.Disconnect(this, &ResourcesBrowser::onResourceDatabaseAdded);
		database = nullptr;
		return;
	}

	database = db;
	database->onResourceAdded.Connect(this, &ResourcesBrowser::onResourceDatabaseAdded);
}

//-----------------------------------//

void ResourcesBrowser::onResourceDatabaseAdded(const ResourceMetadata& md)
{
	// If a resource is added to the database, check if we need to add it to the list.
	if(selectedGroup != ResourceGroup::General && md.group != selectedGroup) return;

	addResource(md);
}

//-----------------------------------//

void ResourcesBrowser::setupUI()
{
	CenterOnParent();
	
	wxIcon icon;
	icon.CopyFromBitmap( wxMEMORY_BITMAP(package) );
	SetIcon(icon);
}

//-----------------------------------//

void ResourcesBrowser::enableSelection()
{
	inSelectionMode = true;
	m_resourceGroups->Disable();
}

//-----------------------------------//

void ResourcesBrowser::disableSelection()
{
	inSelectionMode = false;
	m_resourceGroups->Enable();
}

//-----------------------------------//

void ResourcesBrowser::onResourceListActivated( wxListEvent& event )
{
	if( !inSelectionMode ) return;
	
	EndModal( event.GetIndex() );
}

//-----------------------------------//

#define RG(T) ResourceGroup::##T
#define RGI(T) m_resourceGroupImagesMap[RG(T)]

void ResourcesBrowser::setupGroups()
{
	m_rootId = m_resourceGroups->AddRoot("Resources", RGI(General));
	m_resourceGroupIds[RG(General)] = m_resourceGroups->AppendItem(m_rootId, "All", RGI(General));

	Enum* enuhm = ReflectionGetType(ResourceGroup);
	const EnumValuesMap& values = enuhm->values;
	
	for( auto it = values.begin(); it != values.end(); it++ )
	{
		ResourceGroup::Enum group = (ResourceGroup::Enum) it->second;
		if(group == ResourceGroup::General) continue;

		int image = m_resourceGroupImagesMap[group];
		
		wxTreeItemId id = m_resourceGroups->AppendItem(m_rootId, it->first, image);
		m_resourceGroupIds[group] = id;
	}
}

//-----------------------------------//

#define CREATE_RESOURCE_ICON(T, I) \
	m_resourceGroupImagesMap[RG(T)] = m_resourceGroupsImages->Add(wxMEMORY_BITMAP(I));	

void ResourcesBrowser::setupGroupIcons()
{
	m_resourceGroupsImages = new wxImageList(16, 16, false, 8);
	m_resourceGroupsImages->Add(wxMEMORY_BITMAP(bullet_blue));
	CREATE_RESOURCE_ICON( General, package )
	CREATE_RESOURCE_ICON( Images, image )
	CREATE_RESOURCE_ICON( Meshes, shape_flip_horizontal )
	CREATE_RESOURCE_ICON( Fonts, font )
	CREATE_RESOURCE_ICON( Shaders, script_palette )
	CREATE_RESOURCE_ICON( Audio, music )
	CREATE_RESOURCE_ICON( Scripts, page_code )
	CREATE_RESOURCE_ICON( Materials, palette )
	m_resourceGroups->AssignImageList(m_resourceGroupsImages);
}

//-----------------------------------//

bool ResourcesBrowser::findResourceGroupFromTreeId( wxTreeItemId id, ResourceGroup::Enum& group )
{
	for(auto it = m_resourceGroupIds.begin(); it != m_resourceGroupIds.end(); it++)
	{
		if(it->second != id) continue;
		
		group = it->first;
		return true;
	}

	return false;
}

//-----------------------------------//

void ResourcesBrowser::selectGroup(ResourceGroup::Enum group)
{
	selectedGroup = group;

	wxTreeItemId id = m_resourceGroupIds[group];
	m_resourceGroups->SelectItem(id);
}

//-----------------------------------//

void ResourcesBrowser::onResourceGroupChanged( wxTreeEvent& event )
{
	wxTreeItemId id = event.GetItem();
	ResourceGroup::Enum group;
	
	if( !findResourceGroupFromTreeId(id, group) )
		return;

	showGroup(group);
}

//-----------------------------------//

#define LIST_STYLE wxLC_NO_HEADER | wxLC_SINGLE_SEL

void ResourcesBrowser::showGroup(ResourceGroup::Enum group)
{
	int imageSize = m_detailSlider->GetValue();
	bool showImages = imageSize > 0;

	m_resourceList->ClearAll();

	m_resourceImages = new wxImageList(imageSize, imageSize);
	m_resourceList->AssignImageList(m_resourceImages, wxIMAGE_LIST_NORMAL);

	listIndex = 0;
	m_resourceList->DeleteAllColumns();
		
	if( showImages )
		m_resourceList->SetWindowStyle(wxLC_ICON | LIST_STYLE);
	else
		m_resourceList->SetWindowStyleFlag(wxLC_REPORT | LIST_STYLE);

	wxListItem col; 
	col.SetId(0); 
	col.SetText("Resource"); 
	col.SetWidth(300);
		
	m_resourceList->InsertColumn(0, col);

	ResourcesPlugin* rp = GetPlugin<ResourcesPlugin>();
	
	ResourceDatabase* db = rp->resourceDatabase;
	
	if( !db )
	{
		LogAssert("No resource database found");
		return;
	}

	addDatabaseGroup(db, group);
}

//-----------------------------------//

void ResourcesBrowser::addDatabaseGroup(ResourceDatabase* db, ResourceGroup::Enum group)
{
	bool isGeneral = group == ResourceGroup::General;

	ResourcesCacheMap& cache = db->resourcesCache;

	for(auto it = cache.begin(); it != cache.end(); it++)
	{
		ResourceMetadata& metadata = it->second;

		if(!isGeneral && (metadata.group != group))
			continue;

		addResource(metadata);
	}
}

//-----------------------------------//

void ResourcesBrowser::addResource(const ResourceMetadata& metadata)
{
	int imageSize = m_detailSlider->GetValue();
	bool showImages = imageSize > 0;

	String name = PathGetFile(metadata.path);

	if(!showImages)
	{
		m_resourceList->InsertItem(listIndex++, name);
	}
	else
	{
		int index = getImageIndex(metadata);
		if( index < 0 ) return;
			
		long id = m_resourceList->InsertItem(listIndex++, name);
		m_resourceList->SetItemImage(id, index);
	}
}

//-----------------------------------//

int ResourcesBrowser::getImageIndex( const ResourceMetadata& metadata )
{
	int imageSize = m_detailSlider->GetValue();

	String fullPath = CacheFolder + metadata.preview;

	if(metadata.group == ResourceGroup::Images)
		fullPath = MediaFolder + metadata.path;

	if( !wxFileName::FileExists(fullPath) )
		return -1;

	wxImage image;

	if( !image.LoadFile(fullPath) )
		return -1;

	image.Rescale(imageSize, imageSize, wxIMAGE_QUALITY_NEAREST);

	int index = m_resourceImages->Add( wxBitmap(image) );

	return index;
}

//-----------------------------------//

void ResourcesBrowser::onResourceListSelection( wxListEvent& event )
{
	int id = event.GetIndex();
	wxString selection = m_resourceList->GetItemText(id);
	
	String path = String(selection.c_str());
	const ResourcePtr& res = GetResourceManager()->loadResource(path).Resolve();

	if( !inSelectionMode )
	{
		EventManager* events = GetEditor().getEventManager();
		events->onResourceSelect(res);
	}
}

//-----------------------------------//

void ResourcesBrowser::onResourceSliderScroll( wxScrollEvent& event )
{
	int value = event.GetInt();
	int remainder = value % 32;
	
	value -= remainder;
	m_detailSlider->SetValue(value);

	wxTreeItemId id = m_resourceGroups->GetSelection();
	if( !id.IsOk() ) return;

	ResourceGroup::Enum group;

	if( !findResourceGroupFromTreeId(id, group) )
		return;

	showGroup(group);
}

//-----------------------------------//

static Vector3 CalculatePlacementCoords(const Vector2& coords)
{
	SceneDocument* document = (SceneDocument*) GetEditor().getDocument();
	
	Scene* scene = document->scene.get();
	Camera* camera = document->getViewframe()->getCamera().get();

	Ray ray = camera->getRay(coords.x, coords.y);
	RayTriangleQueryResult res;

	Vector3 dropPoint;

	if( scene->doRayTriangleQuery(ray, res) )
	{
		dropPoint = res.intersectionWorld;
	}
	else
	{
		Plane ground( Vector3::UnitY, 0 );
		
		float distance;
		
		if( !ground.intersects(ray, distance) )
			return Vector3::Zero;
			
		dropPoint = ray.getPoint(distance);
	}

	return dropPoint;
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

	String name = event.GetText();

	ResourceManager* rm = GetResourceManager();
	
	ResourceHandle handle = rm->loadResource(name);
	Resource* resource = handle.Resolve();

	if( !resource || resource->getResourceGroup() != ResourceGroup::Meshes )
	{
		// Resource is not a mesh.
		return;
	}

	MeshHandle mesh = HandleCast<Mesh>(handle);
	if( !mesh ) return;

	Entity* entity = EntityCreate( AllocatorGetHeap() );
	entity->setName(PathGetFile(name));
	entity->addTransform();

	Vector2 coords = GetEditor().getDropCoords();
	Vector3 dropPoint = CalculatePlacementCoords(coords);

	entity->getTransform()->setPosition(dropPoint);

	Model* model = AllocateThis(Model);
	model->setMesh(mesh);

	entity->addComponent(model);

	SceneDocument* document = (SceneDocument*) GetEditor().getDocument();
	Scene* scene = document->scene.get();

	EntityOperation* entityOperation = AllocateThis(EntityOperation);
	entityOperation->type = EntityOperation::EntityAdded;
	entityOperation->description = "Entity dragged";
	entityOperation->entity = entity;
	entityOperation->weakScene = scene;
	entityOperation->redo();

	UndoManager* undoManager = document->getUndoManager();
	undoManager->registerOperation(entityOperation);
}

//-----------------------------------//

void ResourcesBrowser::setFocusToSearch()
{
	m_searchCtrl->SetFocus();
}

//-----------------------------------//

void ResourcesBrowser::OnClose(wxCloseEvent& event)
{
	if ( !event.CanVeto() ) return;

	Hide();

	if( inSelectionMode )
	{
		// When in selection mode, if the user closes the window,
		// then explicitly set the return code of ShowModal()

		SetReturnCode(-1);
	}

	event.Veto();
}

//-----------------------------------//

NAMESPACE_EDITOR_END

#endif