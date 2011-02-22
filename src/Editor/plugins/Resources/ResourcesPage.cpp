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
#include "Events.h"
#include "Utilities.h"

namespace vapor { namespace editor {

//-----------------------------------//

ResourcesPage::ResourcesPage( wxWindow* parent, wxWindowID id,
										const wxPoint& pos, const wxSize& size )
	: wxTreeCtrl(parent, id, pos, size, wxTR_DEFAULT_STYLE | wxTR_HIDE_ROOT)
{
	rm = GetResourceManager();
	
	rm->onResourcePrepared.Connect( this, &ResourcesPage::onResourcePrepared );
	rm->onResourceRemoved.Connect( this, &ResourcesPage::onResourceRemoved );
	
	rm->onResourceLoaded.Connect( this, &ResourcesPage::onResourceReloaded );
	rm->onResourceReloaded.Connect( this, &ResourcesPage::onResourceReloaded );

	initIcons();
	initControl();
	updateTree();
}

//-----------------------------------//

ResourcesPage::~ResourcesPage()
{
	rm->onResourcePrepared.Disconnect( this, &ResourcesPage::onResourcePrepared );
	rm->onResourceRemoved.Disconnect( this, &ResourcesPage::onResourceRemoved );

	rm->onResourceLoaded.Disconnect( this, &ResourcesPage::onResourceReloaded );
	rm->onResourceReloaded.Disconnect( this, &ResourcesPage::onResourceReloaded );
}

//-----------------------------------//

#define RG(T) ResourceGroup::##T

void ResourcesPage::initControl()
{
	rootId = AddRoot( "Resources", resGroupIcons[RG(General)] );
	//resGroupIds[RG(General)] = AppendItem(rootId, "General", resGroupIcons[RG(General)]);

	const Enum& enuhm = ResourceGroup::getStaticType();
	const EnumValuesMap& values = enuhm.getValues();
	
	EnumValuesMap::const_iterator it;
	for( it = values.begin(); it != values.end(); it++ )
	{
		ResourceGroup::Enum group = (ResourceGroup::Enum) it->second;
		resGroupIds[group] = AppendItem(rootId, it->first, resGroupIcons[group]);
	}

	Bind(wxEVT_COMMAND_TREE_SEL_CHANGED, &ResourcesPage::onItemChanged, this);
	Bind(wxEVT_COMMAND_TREE_ITEM_MENU, &ResourcesPage::onTreeItemMenu, this);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &ResourcesPage::onCommandMenuSelected, this);
}

//-----------------------------------//

#define CREATE_RESOURCE_ICON( T, I )							\
	resGroupIcons[RG(T)] = imageList->Add(wxMEMORY_BITMAP(I));	

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
	const ResourceMap& resources = rm->getResources();
	
	ResourceMap::const_iterator it;
	for( it = resources.begin(); it != resources.end(); it++ )
	{
		const ResourcePtr& res = it->second;
		addResource(res);
	}
}

//-----------------------------------//

wxTreeItemId ResourcesPage::addResource(const ResourcePtr& res)
{
	if( !res ) return nullptr;

	ResourceIdsMap::iterator it = resourceIds.find( res.get() );
	
	if( it != resourceIds.end() ) return nullptr;

	ResourceGroup::Enum group = res->getResourceGroup();
	const std::string& resPath = res->getPath();
	
	wxTreeItemId id = AppendItem(resGroupIds[group], resPath, 0 );
	resourceIds[res.get()] = id;

	return id;
}

//-----------------------------------//

void ResourcesPage::onResourcePrepared( const ResourceEvent& event )
{
	const ResourcePtr& res = event.resource;
	addResource(res);
}

//-----------------------------------//

void ResourcesPage::onResourceRemoved( const ResourceEvent& event )
{
	const ResourcePtr& res = event.resource;

	ResourceIdsMap::iterator it = resourceIds.find( res.get() );
	assert( it != resourceIds.end() );

	wxTreeItemId id = (*it).second;
	assert( id.IsOk() );

	Delete(id);
	resourceIds.erase(it);
}

//-----------------------------------//

void ResourcesPage::onResourceReloaded( const ResourceEvent& event )
{
	// Update the view when resources get reloaded.
	GetEditor().redrawView();
}

//-----------------------------------//

ResourcePtr ResourcesPage::getResourceFromTreeId( wxTreeItemId id )
{
	if( !id )
		return nullptr;

	return rm->getResource( (std::string) GetItemText(id) );
}

//-----------------------------------//

void ResourcesPage::onItemChanged( wxTreeEvent& event )
{
	wxTreeItemId oldId = event.GetOldItem();
	wxTreeItemId newId = event.GetItem();

	const ResourcePtr& oldResource = getResourceFromTreeId( oldId );
	const ResourcePtr& newResource = getResourceFromTreeId( newId );

	Events* events = GetEditor().getEventManager();

	if( oldResource )
		events->onResourceUnselect(oldResource);

	if( newResource )
		events->onResourceSelect(newResource);
}

//-----------------------------------//

enum
{
	ID_ResourceMenu_Open = wxID_HIGHEST+982,
	ID_ResourceMenu_Reload,
	ID_ResourceMenu_Unload,
	ID_ResourceMenu_Reimport,
	ID_ResourceMenu_VCS_Update,
	ID_ResourceMenu_VCS_Status,
	ID_ResourceMenu_VCS_Commit,
	ID_ResourceMenu_VCS_ShowLog,
};

static bool isUnderVersionControl(const ResourcePtr& res)
{
	File file( res->getPath() );

	const std::string& fullPath = PathUtils::getCurrentDir()
		+ PathUtils::getPathSeparator()
		+ PathUtils::getBasePath(file.getRealPath())
		+ PathUtils::getPathSeparator()
		+ ".svn";

	return wxFileName::DirExists(fullPath);
}


void ResourcesPage::onTreeItemMenu(wxTreeEvent& event)
{
	menuItemId = event.GetItem();
	
	const ResourcePtr& res = getResourceFromTreeId( menuItemId );

	if( !res )
		return;

	wxMenu menu;

	menu.Append( ID_ResourceMenu_Reimport, "&Reimport" );
	
	menu.AppendSeparator();
	
	menu.Append( ID_ResourceMenu_Open, "&Open" );
	menu.Append( ID_ResourceMenu_Reload, "&Reload" );
	menu.Append( ID_ResourceMenu_Unload, "&Unload" );

	wxMenu* menuVCS = new wxMenu("Subversion");
	menuVCS->Append( ID_ResourceMenu_VCS_Update, "&Update" );
	//menuVCS->Append( ID_ResourceMenu_VCS_Status, "&Status" );
	menuVCS->Append( ID_ResourceMenu_VCS_Commit, "&Commit" );
	menuVCS->Append( ID_ResourceMenu_VCS_ShowLog, "Show &log" );
	menu.AppendSubMenu(menuVCS, "Version Control");

	if( !isUnderVersionControl(res) )
	{
		wxMenuItemList& menus = menuVCS->GetMenuItems();

		for(uint i = 0; i < menus.size(); i++ )
		{
			wxMenuItem* menuItem = menus[i];
			menuItem->Enable(false);
		}
	}

	wxPoint clientpt = event.GetPoint();
	PopupMenu(&menu, clientpt);
}

static std::string getResourceFullPath(const ResourcePtr& res)
{
	File file( res->getPath() );

	const std::string& fullPath = PathUtils::getCurrentDir()
		+ PathUtils::getPathSeparator()
		+ file.getRealPath();

	return fullPath;
}

static wxString getTortoiseBaseCommand(const ResourcePtr& res, const std::string& operation)
{
	wxString command("TortoiseProc.exe");
	command.Append(" /command:");
	command.Append(operation);
	command.Append(" /path:\"");
	command.Append(getResourceFullPath(res));
	command.Append("\"");

	return command;
}

static void executeCommand(const wxString& command)
{
	if( wxExecute(command) == 0 )
	{
		//wxMessageBox(
	}
}

//-----------------------------------//

void ResourcesPage::onCommandMenuSelected( wxCommandEvent& event )
{
	int id = event.GetId();

	ResourcePtr res = getResourceFromTreeId( menuItemId );
	std::string fullPath = getResourceFullPath(res);

	switch(id)
	{
	case ID_ResourceMenu_Open:
	{
		wxLaunchDefaultApplication(fullPath);
		break;
	}
	case ID_ResourceMenu_VCS_Update:
	{
		wxString command = getTortoiseBaseCommand(res, "update");
		executeCommand(command);
		break;
	}
	case ID_ResourceMenu_VCS_Commit:
	{
		wxString command = getTortoiseBaseCommand(res, "commit");
		executeCommand(command);
		break;
	}
	case ID_ResourceMenu_VCS_ShowLog:
	{
		wxString command = getTortoiseBaseCommand(res, "log");
		executeCommand(command);
		break;
	}
	}
}

//-----------------------------------//

} } // end namespaces