/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Editor/API.h"
#include "ResourcesPane.h"
#include "EditorIcons.h"
#include "Editor.h"
#include "Events.h"
#include "Core/Utilities.h"

namespace vapor { namespace editor {

//-----------------------------------//

ResourcesPage::ResourcesPage( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size )
	: wxTreeCtrl(parent, id, pos, size, wxTR_DEFAULT_STYLE | wxTR_HIDE_ROOT)
{
	ResourceManager* res = GetResourceManager();
	
	res->onResourcePrepared.Connect( this, &ResourcesPage::onResourcePrepared );
	res->onResourceRemoved.Connect( this, &ResourcesPage::onResourceRemoved );
	
	res->onResourceLoaded.Connect( this, &ResourcesPage::onResourceReloaded );
	res->onResourceReloaded.Connect( this, &ResourcesPage::onResourceReloaded );

	initIcons();
	initControl();
	updateTree();
}

//-----------------------------------//

ResourcesPage::~ResourcesPage()
{
	ResourceManager* res = GetResourceManager();

	res->onResourcePrepared.Disconnect( this, &ResourcesPage::onResourcePrepared );
	res->onResourceRemoved.Disconnect( this, &ResourcesPage::onResourceRemoved );

	res->onResourceLoaded.Disconnect( this, &ResourcesPage::onResourceReloaded );
	res->onResourceReloaded.Disconnect( this, &ResourcesPage::onResourceReloaded );
}

//-----------------------------------//

#define RG(T) ResourceGroup::##T

void ResourcesPage::initControl()
{
	rootId = AddRoot( "Resources", resGroupIcons[RG(General)] );
	//resGroupIds[RG(General)] = AppendItem(rootId, "General", resGroupIcons[RG(General)]);

	Enum* enuhm = ReflectionGetType(ResourceGroup);
	const EnumValuesMap& values = enuhm->values;
	
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

#define CREATE_RESOURCE_ICON( T, I ) resGroupIcons[RG(T)] = imageList->Add(wxMEMORY_BITMAP(I));	

void ResourcesPage::initIcons()
{
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
	const ResourceMap& resources = GetResourceManager()->getResources();
	
	for( auto it = resources.begin(); it != resources.end(); it++ )
	{
		const ResourceHandle& res = it->second;
		addResource(res);
	}
}

//-----------------------------------//

wxTreeItemId ResourcesPage::addResource(const ResourceHandle& handle)
{
	Resource* res = handle.Resolve();
	if( !res ) return nullptr;

	if( resourceIds.find(res) != resourceIds.end() )
		return nullptr;

	ResourceGroup::Enum group = res->getResourceGroup();
	const Path& resPath = res->getPath();
	
	wxTreeItemId id = AppendItem(resGroupIds[group], resPath, 0 );
	resourceIds[res] = id;

	return id;
}

//-----------------------------------//

void ResourcesPage::onResourcePrepared( const ResourceEvent& event )
{
	const ResourceHandle& handle = event.handle;
	addResource(handle);
}

//-----------------------------------//

void ResourcesPage::onResourceRemoved( const ResourceEvent& event )
{
	const ResourceHandle& handle = event.handle;
	Resource* res = handle.Resolve();

	ResourceIdsMap::iterator it = resourceIds.find(res);
	assert( it != resourceIds.end() );

	wxTreeItemId id = it->second;
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

ResourceHandle ResourcesPage::getResourceFromTreeId( wxTreeItemId id )
{
	if( !id ) return ResourceHandle();

	return GetResourceManager()->getResource( (String) GetItemText(id) );
}

//-----------------------------------//

void ResourcesPage::onItemChanged( wxTreeEvent& event )
{
	wxTreeItemId oldId = event.GetOldItem();
	wxTreeItemId newId = event.GetItem();

	const ResourceHandle& oldResource = getResourceFromTreeId( oldId );
	const ResourceHandle& newResource = getResourceFromTreeId( newId );

	Events* events = GetEditor().getEventManager();

	if( oldResource )
		events->onResourceUnselect(ResourcePtr(oldResource.Resolve()));

	if( newResource )
		events->onResourceSelect(ResourcePtr(newResource.Resolve()));
}

//-----------------------------------//

enum
{
	ID_ResourceMenu_Open = wxID_HIGHEST+982,
	ID_ResourceMenu_Browse,
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
#if 0
	Stream* stream = StreamCreateFromFile( AllocatorGetHeap(), res->getPath() );
	/*, StreamMode::Read );*/

	const String& fullPath = PathGetCurrentDir()
		+ PathGetSeparator()
		+ PathGetBase( FileGetFullPath(&file) )
		+ PathGetSeparator()
		+ ".svn";

	FileClose(&file);

	return wxFileName::DirExists(fullPath);
#endif

	return false;
}


void ResourcesPage::onTreeItemMenu(wxTreeEvent& event)
{
	menuItemId = event.GetItem();
	
	const ResourceHandle& res = getResourceFromTreeId( menuItemId );

	if( !res )
		return;

	wxMenu menu;

	menu.Append( ID_ResourceMenu_Reimport, "&Reimport" );
	
	menu.AppendSeparator();
	
	menu.Append( ID_ResourceMenu_Open, "&Open" );
	menu.Append( ID_ResourceMenu_Browse, "&Browse" );
	menu.Append( ID_ResourceMenu_Reload, "&Reload" );
	menu.Append( ID_ResourceMenu_Unload, "&Unload" );

	wxMenu* menuVCS = new wxMenu("Subversion");
	menuVCS->Append( ID_ResourceMenu_VCS_Update, "&Update" );
	//menuVCS->Append( ID_ResourceMenu_VCS_Status, "&Status" );
	menuVCS->Append( ID_ResourceMenu_VCS_Commit, "&Commit" );
	menuVCS->Append( ID_ResourceMenu_VCS_ShowLog, "Show &log" );
	menu.AppendSubMenu(menuVCS, "Version Control");

	if( !isUnderVersionControl(res.Resolve()) )
	{
		wxMenuItemList& menus = menuVCS->GetMenuItems();

		for(size_t i = 0; i < menus.size(); i++ )
		{
			wxMenuItem* menuItem = menus[i];
			menuItem->Enable(false);
		}
	}

	wxPoint clientpt = event.GetPoint();
	PopupMenu(&menu, clientpt);
}

static String getResourceFullPath(const ResourcePtr& res)
{
	const String& path = res->getPath();

	const String& fullPath = PathGetCurrentDir()
		+ PathGetSeparator()
		+ path;

	return fullPath;
}

static wxString getTortoiseBaseCommand(const ResourcePtr& res, const String& operation)
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

	const ResourceHandle& handle = getResourceFromTreeId( menuItemId );
	Resource* res = handle.Resolve();

	const String& fullPath = getResourceFullPath(res);

	switch(id)
	{
	case ID_ResourceMenu_Open:
	{
		wxLaunchDefaultApplication(fullPath);
		break;
	}
	case ID_ResourceMenu_Browse:
	{
#if 0
		SHELLEXECUTEINFOA info;
		info.cbSize = sizeof(SHELLEXECUTEINFOA);
        info.fMask = 0;
        info.hwnd = nullptr;
        info.lpParameters = nullptr;
        info.lpDirectory = nullptr;
        info.nShow = SW_MAXIMIZE;
        info.hInstApp = nullptr;
		info.lpVerb = "explore";
		info.lpFile = fullPath.c_str();
		
		if( !ShellExecuteExA(&info) )
			return;
#endif
		wxString command = wxString("explorer.exe /select,") + fullPath;
		executeCommand(command);
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