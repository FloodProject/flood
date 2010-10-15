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
#include <wx/utils.h> 

namespace vapor { namespace editor {

//-----------------------------------//

ResourcesPage::ResourcesPage( EditorFrame* editor,
							 wxWindow* parent, wxWindowID id,
							 const wxPoint& pos, const wxSize& size )
	: wxTreeCtrl(parent, id, pos, size, wxTR_DEFAULT_STYLE|wxTR_HIDE_ROOT,
	  wxDefaultValidator, "ResourcesPage")
	, editor(editor)
{
	engine = editor->getEngine();
	rm = engine->getResourceManager();
	
	rm->onResourceAdded += fd::bind( &ResourcesPage::onResourceAdded, this );
	rm->onResourceRemoved += fd::bind( &ResourcesPage::onResourceRemoved, this );
	
	rm->onResourceLoaded += fd::bind( &ResourcesPage::onResourceReloaded, this );
	rm->onResourceReloaded += fd::bind( &ResourcesPage::onResourceReloaded, this );

	initIcons();
	initControl();
	updateTree();
}

//-----------------------------------//

ResourcesPage::~ResourcesPage()
{
	rm->onResourceAdded -= fd::bind( &ResourcesPage::onResourceAdded, this );
	rm->onResourceRemoved -= fd::bind( &ResourcesPage::onResourceRemoved, this );

	rm->onResourceLoaded -= fd::bind( &ResourcesPage::onResourceReloaded, this );
	rm->onResourceReloaded -= fd::bind( &ResourcesPage::onResourceReloaded, this );
}

//-----------------------------------//

#define RG(T) ResourceGroup::##T

void ResourcesPage::initControl()
{
	rootItemId = AddRoot( "Resources", resourceGroupIcons[RG(General)] );

	const Enum& resourcesEnum = ResourceGroup::getType();

	resourceGroupTreeIds[RG(General)] =
		AppendItem(rootItemId, "General", resourceGroupIcons[RG(General)]);

	foreach( const EnumValuesPair& p, resourcesEnum.getValues() )
	{
		ResourceGroup::Enum value = (ResourceGroup::Enum) p.second;

		if( value == ResourceGroup::General )
			continue;

		resourceGroupTreeIds[value] =
			AppendItem(rootItemId, p.first, resourceGroupIcons[value]);
	}

	Bind(wxEVT_COMMAND_TREE_ITEM_MENU, &ResourcesPage::onTreeItemMenu, this);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &ResourcesPage::onCommandMenuSelected, this);
}

//-----------------------------------//

#define CREATE_RESOURCE_ICON( T, I )			\
	resourceGroupIcons[RG(T)] =					\
			imageList->Add(wxMEMORY_BITMAP(I));	

void ResourcesPage::initIcons()
{
	// create a new list of all the icons
	imageList = new wxImageList(16, 16, false, 8);

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

void ResourcesPage::updateTree()
{
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
	editor->redrawView();
}

//-----------------------------------//

ResourcePtr ResourcesPage::getResourceFromTreeId( wxTreeItemId id )
{
	if( !id )
		return nullptr;

	return rm->getResource( (std::string) GetItemText(id) );
}

//-----------------------------------//

enum
{
	ID_ResourceMenu_Open = wxID_HIGHEST+982,
	ID_ResourceMenu_Reload,
	ID_ResourceMenu_Unload,
};

void ResourcesPage::onTreeItemMenu(wxTreeEvent& event)
{
	menuItemId = event.GetItem();
	
	ResourcePtr res = getResourceFromTreeId( menuItemId );

	if( !res )
		return;

	wxMenu menu("Resource");

	menu.Append( ID_ResourceMenu_Open, "&Open" );
	menu.Append( ID_ResourceMenu_Reload, "&Reload" );
	menu.Append( ID_ResourceMenu_Unload, "&Unload" );

	wxPoint clientpt = event.GetPoint();
	PopupMenu(&menu, clientpt);
}

//-----------------------------------//

void ResourcesPage::onCommandMenuSelected( wxCommandEvent& event )
{
	int id = event.GetId();

	ResourcePtr res = getResourceFromTreeId( menuItemId );

	if( !res )
		return;

	if( id == ID_ResourceMenu_Open )
	{
		File file( res->getPath() );

		if( !wxLaunchDefaultApplication( file.getRealPath() ) )
			Log::debug("Failed to open resource '%s'", file.getName().c_str() );
	}
}

//-----------------------------------//

} } // end namespaces