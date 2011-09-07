/************************************************************************
*
* vapor3D Editor � (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Editor/API.h"
#include "ResourcesPlugin.h"
#include "ResourcesPane.h"
#include "ResourcesBrowser.h"
#include "Resources/ResourceIndexer.h"
#include "Resources/ResourceDatabase.h"
#include "ResourceThumbnailer.h"
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
	, resourceIndexer(nullptr)
	, resourceThumbnailer(nullptr)
{
}

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
	metadata.priority = 400;

	return metadata;
}

//-----------------------------------//

void ResourcesPlugin::onPluginEnable()
{
	wxBitmap iconPackage = wxMEMORY_BITMAP(package);

#if ENABLE_RESOURCES_PANE
	resourcesPage = AllocateThis(ResourcesPage, editor);
	resourcesPage->SetSize(300, 200);
	
	wxAuiPaneInfo pane;
	pane.Caption("Resources").Left().Dock().Icon(iconPackage).PaneBorder(true);
	editor->getAUI()->AddPane(resourcesPage, pane);
	editor->getAUI()->Update();
#endif

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

	if( !loadCache() )
	{
		resourceIndexer = AllocateThis(ResourceIndexer);
		
		resourceDatabase = AllocateThis(ResourceDatabase);
		resourceDatabase->setIndexer(resourceIndexer);

		resourceThumbnailer = AllocateThis(ResourceThumbnailer);
		resourceThumbnailer->setIndexer(resourceIndexer);

		Archive* archive = ArchiveCreateFromDirectory(AllocatorGetThis(), MediaFolder);
		resourceIndexer->addArchive(archive);
		Deallocate(archive);
	}

	PipelineInit();
}

//-----------------------------------//

void ResourcesPlugin::onPluginDisable()
{
	if(resourcesPage)
	{
		editor->getAUI()->DetachPane(resourcesPage);
		editor->getAUI()->Update();
	}

	Deallocate(resourceIndexer);
	Deallocate(resourceDatabase);
	Deallocate(resourceDatabase);
	if(resourcesPage) resourcesPage->Destroy();
	//Deallocate(resourcesPage);

	PipelineCleanup();
}

//-----------------------------------//

void ResourcesPlugin::onPluginUpdate()
{
	resourceIndexer->update();
	resourceThumbnailer->update();
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
#ifdef ENABLE_RESOURCE_BROWSER
	resourcesBrowser->Show( !resourcesBrowser->IsVisible() );
	resourcesBrowser->setFocusToSearch();
#endif
}

//-----------------------------------//

NAMESPACE_EDITOR_END