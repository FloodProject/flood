/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Editor/API.h"
#include "ResourcesPlugin.h"
#include "ResourcesPane.h"
#include "ResourcesBrowser.h"
#include "Resources/ResourceDatabase.h"
#include "Settings.h"

#include "Editor.h"
#include "EditorIcons.h"
#include "Pipeline/ResourceProcessor.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(ResourcesPlugin, EditorPlugin)
REFLECT_CLASS_END()

//-----------------------------------//

ResourcesPlugin::ResourcesPlugin()
	: resourcesPage(nullptr)
	, resourcesBrowser(nullptr)
	, resourceDatabase(nullptr)
	, resourceThumbnailer(nullptr)
{ }

//-----------------------------------//

ResourcesPlugin::~ResourcesPlugin()
{
}

//-----------------------------------//

PluginMetadata ResourcesPlugin::getMetadata()
{
	PluginMetadata metadata;
	
	metadata.name = "Resources";
	metadata.description = "Provides resource handling features.";
	metadata.author = "triton";
	metadata.version = "1.0";

	return metadata;
}

//-----------------------------------//

void ResourcesPlugin::onPluginEnable()
{
#if 0
	resourcesPage = Allocate(ResourcesPage, AllocatorGetHeap(), editor);
	resourcesPage->SetSize(300, 200);
#endif

	wxBitmap iconPackage = wxMEMORY_BITMAP(package);

#if 0
	wxAuiPaneInfo pane;
	pane.Caption("Resources").Left().Dock().Icon(iconPackage).PaneBorder(true);
	editor->getAUI()->AddPane(resourcesPage, pane);
	editor->getAUI()->Update();
#endif

	if( !loadCache() )
		resourceDatabase = new ResourceDatabase();

#ifdef ENABLE_RESOURCE_BROWSER
	wxAuiToolBar* toolbarCtrl = editor->getToolbar();

	if(toolbarCtrl)
	{
		resourcesBrowserButton = toolbarCtrl->AddTool( wxID_ANY, "Resources Browser", iconPackage );
		addTool( resourcesBrowserButton );
		int id = resourcesBrowserButton->GetId();
		toolbarCtrl->Bind(wxEVT_COMMAND_TOOL_CLICKED, &ResourcesPlugin::onBrowserButtonClick, this, id);
	}

	resourcesBrowser = new ResourcesBrowser(editor);
#endif

	Pipeline::Init();
}

//-----------------------------------//

void ResourcesPlugin::onPluginDisable()
{
	if(resourcesPage)
	{
		editor->getAUI()->DetachPane(resourcesPage);
		editor->getAUI()->Update();
	}

	Deallocate(resourcesBrowser);
	resourcesBrowser = nullptr;

	Deallocate(resourceDatabase);
	resourceDatabase = nullptr;

	Deallocate(resourcesPage);
	resourcesPage = nullptr;

	Pipeline::Cleanup();
}

//-----------------------------------//

bool ResourcesPlugin::loadCache()
{
	String path = CacheFolder + ThumbCache;

	if( !wxFileName::DirExists(CacheFolder) )
	{
		LogInfo("Creating cache directory: '%s'", CacheFolder.c_str());
		wxFileName::Mkdir(CacheFolder);
		return false;
	}

	if( !FileExists(path) )
	{
		LogWarn("Could not find thumbnails cache file '%s'", path.c_str());
		return false;
	}

	Serializer* serializer = SerializerCreateJSON( AllocatorGetThis() );
	resourceDatabase = (ResourceDatabase*) SerializerLoadObjectFromFile(serializer, path);
	Deallocate(serializer);

	if(resourceDatabase)
		LogInfo("Loaded thumbnails cache from '%s'", path.c_str());
	else
		resourceDatabase = Allocate(ResourceDatabase, AllocatorGetThis());

	return true;
}

//-----------------------------------//

bool ResourcesPlugin::saveCache()
{
	String path = CacheFolder + ThumbCache;
	
	Serializer* serializer = SerializerCreateJSON( AllocatorGetThis() );
	
	if( !SerializerSaveObjectToFile(serializer, path, this) )
	{
		Deallocate(serializer);
		return false;
	}

	Deallocate(serializer);
	
	LogInfo("Wrote thumbnails cache to '%s'", path.c_str());
	return true;
}

//-----------------------------------//

void ResourcesPlugin::onBrowserButtonClick(wxCommandEvent& event)
{
	resourcesBrowser->Show( !resourcesBrowser->IsVisible() );
	resourcesBrowser->setFocusToSearch();
}

//-----------------------------------//

NAMESPACE_EDITOR_END