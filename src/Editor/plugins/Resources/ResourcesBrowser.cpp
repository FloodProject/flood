/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Editor/API.h"
#include "ResourcesBrowser.h"
#include "Viewframe.h"
#include "EditorIcons.h"
#include "UndoManager.h"
#include "Settings.h"
#include "Editor.h"
#include "Events.h"
#include "Core/Utilities.h"
#include "../Scene/ScenePane.h"
#include "../Scene/SceneDocument.h"
#include "Core/PluginManager.h"
#include "ResourcesPlugin.h"
#include "ResourceDatabase.h"
#include "../Property/PropertyPlugin.h"
#include "../Property/PropertyPage.h"

#ifdef ENABLE_RESOURCE_BROWSER

namespace vapor { namespace editor {

//-----------------------------------//

ResourcesBrowser::ResourcesBrowser( wxWindow* parent )
	: gui::ResourcesBrowser(parent, wxID_ANY, "Resources Browser")
	, m_resourceImages(nullptr)
	, m_resourceGroupsImages(nullptr)
	, inSelectionMode(false)
{
	setupUI();
	setupGroupIcons();
	setupGroups();

	// Select the general tree item.
	selectGroup(ResourceGroup::General);
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
	ResourcesPlugin* rp = GetPlugin<ResourcesPlugin>();
	ResourceDatabase* db = rp->resourceDatabase;
	ResourcesCacheMap& cache = db->resourcesCache;
	
	bool isGeneral = group == ResourceGroup::General;

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

	for(auto it = cache.begin(); it != cache.end(); it++)
	{
		ResourceMetadata& metadata = it->second;
		
		if(!isGeneral && (metadata.group != group))
			continue;

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
}

//-----------------------------------//

int ResourcesBrowser::getImageIndex( const ResourceMetadata& metadata )
{
	int imageSize = m_detailSlider->GetValue();

	String fullPath = CacheFolder + metadata.thumbnail;

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
		Events* events = GetEditor().getEventManager();
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

void ResourcesBrowser::OnListBeginDrag(wxListEvent& event)
{
	wxTextDataObject data;
	wxDropSource dragSource(this);
	dragSource.SetData(data);
    
	wxDragResult result = dragSource.DoDragDrop( wxDrag_DefaultMove );

	if( result == wxDragCancel || result == wxDragNone )
		return;

	Vector3 dropPoint;

	Vector2 coords = GetEditor().getDropCoords();

	SceneDocument* document = (SceneDocument*) GetEditor().getDocument();
	RenderView* view = document->getViewframe()->view;

	Ray ray = view->getCamera()->getRay(coords.x, coords.y);

	ScenePtr scene = document->scene;
	RayTriangleQueryResult res;

	if( scene->doRayTriangleQuery(ray, res) )
	{
		dropPoint = res.intersectionWorld;
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

	ResourceManager* rm = GetResourceManager();
	MeshHandle mesh = rm->loadResource<Mesh>(name);

	if( !mesh ) return;

	EntityPtr entity( new Entity( PathGetFile(name) ) );
	entity->addTransform();
	entity->getTransform()->setPosition(dropPoint);
	entity->addComponent( ModelPtr( new Model(mesh) ) );

	EntityOperation* nodeOperation = new EntityOperation();
	nodeOperation->entity = entity;
	nodeOperation->weakScene = scene;

	UndoManager* undoManager = document->getUndoManager();
	undoManager->registerOperation(nodeOperation);

	nodeOperation->redo();
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
    event.Veto();
}

//-----------------------------------//

} } // end namespaces

#endif