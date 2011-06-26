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
#include "PluginManager.h"
#include "ResourcesPlugin.h"
#include "ResourceDatabase.h"
#include "../Property/PropertyPlugin.h"
#include "../Property/PropertyPage.h"

#ifdef ENABLE_RESOURCE_BROWSER

namespace vapor { namespace editor {

//-----------------------------------//

ResourcesBrowser::ResourcesBrowser( wxWindow* parent )
	: gui::ResourcesBrowser(parent, wxID_ANY, "Resources Browser")
	, listIndex(0)
{
	setupUI();
	setupGroupIcons();
	setupGroups();

	// Select the general tree item.
	wxTreeItemId id = m_resourceGroupIds[ResourceGroup::General];
	//m_resourceGroups->SelectItem(id);
}

//-----------------------------------//

void ResourcesBrowser::setupUI()
{
	CenterOnParent();

	Bind( wxEVT_CLOSE_WINDOW, &ResourcesBrowser::OnClose, this );

	m_searchCtrl->ShowCancelButton(true);
	m_searchCtrl->ShowSearchButton(true);
	
	wxIcon icon;
	icon.CopyFromBitmap( wxMEMORY_BITMAP(package) );
	SetIcon(icon);
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

void ResourcesBrowser::onResourceGroupChanged( wxTreeEvent& event )
{
	wxTreeItemId id = event.GetItem();
	ResourceGroup::Enum group;
	
	if( !findResourceGroupFromTreeId(id, group) )
		return;

	showCategory(group);
}

//-----------------------------------//

void ResourcesBrowser::showCategory(ResourceGroup::Enum group)
{
	listIndex = 0;
	m_resourceList->Clear();

	ResourcesPlugin* rp = GetPlugin<ResourcesPlugin>();
	ResourceDatabase* db = rp->resourceDatabase;

	ResourcesCacheMap& cache = db->resourcesCache;
	
	bool isGeneral = group == ResourceGroup::General;

	for(auto it = cache.begin(); it != cache.end(); it++)
	{
		ResourceMetadata& metadata = it->second;
		
		if(!isGeneral && (metadata.group != group))
			continue;

		String name = PathGetFile(metadata.path);
		m_resourceList->Append(name);
	}
}

//-----------------------------------//

void ResourcesBrowser::onResourceListSelection( wxCommandEvent& event )
{
	wxString selection = m_resourceList->GetString(event.GetInt());
	String path = String(selection.c_str());

	const ResourcePtr& res = GetResourceManager()->loadResource(path).Resolve();

	Events* events = GetEditor().getEventManager();
	events->onResourceSelect(res);
}

//-----------------------------------//

void ResourcesBrowser::setupImages()
{
#if 0
	int size = ThumbSize / 2;

	for( auto it = resourcesCache.begin(); it != resourcesCache.end(); it++ )
	{
		ResourceMetadata& metadata = it->second;
		
		wxImage image;
		
		if( !image.LoadFile(CacheFolder + metadata.thumbnail) )
			continue;

		image.Rescale(size, size);
		image = image.Mirror(false);

		metadata.index = images->Add( wxBitmap(image) );
		
		String base = PathGetBase(metadata.thumbnail);
		m_resourceList->Insert(listIndex++, base, metadata.index);
	}
#endif
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

	if( !mesh )
		return;

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

void ResourcesBrowser::onConnectClicked(wxCommandEvent& event)
{
#if 0
	if( network.createClientSocket("tcp://127.0.0.1:7654") )
	{
		m_button1->SetLabel("Connected");
		m_button1->Disable();
	}

	std::vector<byte> data;
	data.push_back( MessageType::ResourceIndexRequest );

	MessagePtr message = new Message(data);
	//message->setMessageType( MessageType::ResourceIndexRequest );

	network.sendMessage(message);
#endif
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

#endif