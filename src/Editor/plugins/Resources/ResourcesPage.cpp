/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "PCH.h"
#include "ResourcesPage.h"
#include "EditorIcons.h"
#include "Editor.h"

namespace vapor { namespace editor {

////////////////////////////////////////////////////////////
// Event table
////////////////////////////////////////////////////////////

enum 
{
	ID_ResourceTree,
	//ID_MenuSceneNodeDelete = wxID_DELETE
};

BEGIN_EVENT_TABLE(ResourcesPage, wxTreeCtrl)
	//EVT_TREE_ITEM_MENU(ID_SceneTree, ResourcesPage::onItemMenu)
END_EVENT_TABLE()

//-----------------------------------//

ResourcesPage::ResourcesPage( EditorFrame* editor,
							 wxWindow* parent, wxWindowID id,
							 const wxPoint& pos, const wxSize& size )
	: wxTreeCtrl(parent, id, pos, size,
		wxTR_DEFAULT_STYLE | wxTR_HIDE_ROOT,
		wxDefaultValidator, "ResourcesPage")
	, editor(editor)
{
	engine = editor->getEngine();
	assert( engine != nullptr );

	rm = engine->getResourceManager();
	assert( rm != nullptr );
	
	rm->onResourceAdded += fd::bind( &ResourcesPage::onResourceAdded, this );
	rm->onResourceRemoved += fd::bind( &ResourcesPage::onResourceRemoved, this );
	
	rm->onResourceLoaded += fd::bind( &ResourcesPage::onResourceReloaded, this );
	rm->onResourceReloaded += fd::bind( &ResourcesPage::onResourceReloaded, this );

	initIcons();
	initControl();

	ExpandAll();
}

//-----------------------------------//

ResourcesPage::~ResourcesPage()
{
	assert( rm != nullptr );

	//rm->onResourceAdded -= fd::bind( &ResourcesPage::onResourceAdded, this );
	//rm->onResourceRemoved -= fd::bind( &ResourcesPage::onResourceRemoved, this );

	//rm->onResourceLoaded -= fd::bind( &ResourcesPage::onResourceReloaded, this );
	//rm->onResourceReloaded -= fd::bind( &ResourcesPage::onResourceReloaded, this );
}

//-----------------------------------//

void ResourcesPage::updateTree()
{
	// traverse each resource and add nodes
	foreach( const ResourceMapPair& resource, rm->getResources() )
	{
		const ResourcePtr& res = resource.second;
		ResourceGroup::Enum group = res->getResourceGroup();

		AppendItem( resourceGroupTreeIds[group],
					resource.first, 0
					/*resourceGroupIcons[group]*/ );
	}
}

//-----------------------------------//

void ResourcesPage::onResourceAdded( const ResourceEvent& event )
{
	const ResourcePtr& res = event.resource;
	ResourceGroup::Enum group = res->getResourceGroup();

	AppendItem( resourceGroupTreeIds[group],
				res->getPath(), 0
				/*resourceGroupIcons[group]*/ );
}

//-----------------------------------//

void ResourcesPage::onResourceRemoved( const ResourceEvent& event )
{
}

//-----------------------------------//

void ResourcesPage::onResourceReloaded( const ResourceEvent& event )
{
	// Update the view when resources get reloaded.
	editor->RefreshViewport();
}

//-----------------------------------//

void ResourcesPage::onItemMenu(wxTreeEvent& WXUNUSED(event))
{
//    wxTreeItemId itemId = event.GetItem();
//	//debug( "%s", itemId.IsOk() ? "true" : "false" );
//    //MyTreeItemData *item = itemId.IsOk() ? (MyTreeItemData *)GetItemData(itemId)
//                                         //: nullptr;
//    wxPoint clientpt = event.GetPoint();
//
//#if wxUSE_MENUS
//    wxMenu menu("Scene node");
//    menu.Append(ID_MenuSceneNodeDelete, "&Delete...");
//
//    PopupMenu(&menu, clientpt);
//#endif // wxUSE_MENUS
//
//    event.Skip();
}

//-----------------------------------//

#define RG( T ) ResourceGroup::##T

#define CREATE_RESOURCE_ICON( T, I )			\
	resourceGroupIcons[RG(T)] =					\
			imageList->Add(wxMEMORY_BITMAP(I));	

void ResourcesPage::initIcons()
{
	// create a new list of all the icons
	imageList = new wxImageList(16, 16, false, 10);

	imageList->Add(wxMEMORY_BITMAP(bullet_blue));
	CREATE_RESOURCE_ICON( General, package )
	CREATE_RESOURCE_ICON( Images, image )
	CREATE_RESOURCE_ICON( Meshes, shape_flip_horizontal )
	CREATE_RESOURCE_ICON( Fonts, font )
	CREATE_RESOURCE_ICON( Shaders, palette )
	CREATE_RESOURCE_ICON( Audio, music )
	CREATE_RESOURCE_ICON( Scripts, page_code )

	AssignImageList(imageList);
}

//-----------------------------------//

#define CREATE_RESOURCE_NODE( T )					\
	resourceGroupTreeIds[RG(T)] = AppendItem(root,	\
		ResourceGroup::getString(RG(T)),			\
		resourceGroupIcons[RG(T)] );

void ResourcesPage::initControl()
{
	root = AddRoot( "Resources", resourceGroupIcons[RG(General)] );

	// Add the root nodes
	CREATE_RESOURCE_NODE( General )
	CREATE_RESOURCE_NODE( Images )
	CREATE_RESOURCE_NODE( Meshes )
	CREATE_RESOURCE_NODE( Fonts )
	CREATE_RESOURCE_NODE( Shaders )
	CREATE_RESOURCE_NODE( Audio )
	CREATE_RESOURCE_NODE( Scripts )
}

//-----------------------------------//

} } // end namespaces