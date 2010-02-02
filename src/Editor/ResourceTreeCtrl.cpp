/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "PCH.h"
#include "vapor/Utilities.h"
#include "ResourceTreeCtrl.h"
#include "EditorIcons.h"

using namespace vapor;
using namespace vapor::resources;

namespace vapor { namespace editor {

//typedef std::pair< const std::string, ComponentPtr > ComponentMapPair;

//-----------------------------------//

////////////////////////////////////////////////////////////
// Event table
////////////////////////////////////////////////////////////
BEGIN_EVENT_TABLE(ResourceTreeCtrl, wxTreeCtrl)
	//EVT_TREE_ITEM_MENU(ID_SceneTree, ResourceTreeCtrl::onItemMenu)
END_EVENT_TABLE()

//-----------------------------------//

ResourceTreeCtrl::ResourceTreeCtrl(vapor::Engine* engine, 
					wxWindow* parent, wxWindowID id,
					const wxPoint& pos,
					const wxSize& size,
					long style,
					const wxValidator& validator, 
					const wxString&	name)
	: wxTreeCtrl(parent, id, pos, size, style, validator, name),
		engine(engine), rm( nullptr )
{
	if( !engine )
	{
		assert( "Invalid engine instance." );
	}

	rm = engine->getResourceManager();
	
	initIcons();
	InitControl();

	ExpandAll();
}

//-----------------------------------//

ResourceTreeCtrl::~ResourceTreeCtrl()
{

}

//-----------------------------------//

#define RG( T ) ResourceGroup::##T

#define CREATE_RESOURCE_ICON( T, I )			\
	resourceGroupIcons[RG(T)] =					\
			imageList->Add(wxMEMORY_BITMAP(I));	

void ResourceTreeCtrl::initIcons()
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

void ResourceTreeCtrl::InitControl()
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

	rm->onResourceAdded += fd::bind( &ResourceTreeCtrl::onResourceAdded, this );
	rm->onResourceRemoved += fd::bind( &ResourceTreeCtrl::onResourceRemoved, this );
}

//-----------------------------------//

void ResourceTreeCtrl::updateTree()
{
	typedef std::pair< const std::string, ResourcePtr > resourceMapPair;

	// traverse each resource and add nodes
	foreach( resourceMapPair resource, rm->getResources() )
	{
		ResourcePtr res = resource.second;
		ResourceGroup::Enum group = res->getResourceGroup();

		AppendItem( resourceGroupTreeIds[group],
					resource.first,
					0 /*resourceGroupIcons[group]*/ );
	}
}

//-----------------------------------//

void ResourceTreeCtrl::onItemMenu(wxTreeEvent& WXUNUSED(event))
{
//    wxTreeItemId itemId = event.GetItem();
//	//debug( "%s", itemId.IsOk() ? "true" : "false" );
//    //MyTreeItemData *item = itemId.IsOk() ? (MyTreeItemData *)GetItemData(itemId)
//                                         //: NULL;
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

void ResourceTreeCtrl::onResourceAdded( const resources::ResourceEvent& event )
{
	ResourceGroup::Enum group = event.resource->getResourceGroup();

	AppendItem( resourceGroupTreeIds[group],
				event.name, 0
				/*, resourceGroupIcons[group]*/ );	
}

//-----------------------------------//

void ResourceTreeCtrl::onResourceRemoved( const resources::ResourceEvent& event )
{

}

//-----------------------------------//

} } // end namespaces