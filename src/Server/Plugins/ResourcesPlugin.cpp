/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Server/API.h"
#include "Server/ServerPlugin.h"
#include "Server/Settings.h"
#include "Server/Server.h"

#include "Core/WebServer.h"
#include "Core/Archive.h"
#include "Core/Serialization.h"
#include "Core/SerializationHelpers.h"

#include "Resources/ResourceManager.h"
#include "Resources/ResourceDatabase.h"
#include "Resources/ResourceIndexer.h"
#include "Resources/ResourceLoader.h"
#include "Engine/API.h"

#include "mongoose.h"

NAMESPACE_SERVER_BEGIN

//-----------------------------------//

class ResourcesPlugin : public ServerPlugin
{
public:

	ResourcesPlugin();

	// Gets metadata about this plugin.
	PluginMetadata getMetadata() OVERRIDE;

	// Plugin callbacks.
	void onPluginEnable() OVERRIDE;
	void onPluginDisable() OVERRIDE;
	void update() OVERRIDE;

	void onResourceRequest(WebContext*, WebRequest*);

	WebContext* webContext;

	Archive* archive;
	ResourceManager* resources;
	ResourceDatabase* database;
	ResourceIndexer* indexer;
};

REFLECT_CHILD_CLASS(ResourcesPlugin, ServerPlugin)
REFLECT_CLASS_END()

//-----------------------------------//

ResourcesPlugin::ResourcesPlugin()
	: archive(nullptr)
	, resources(nullptr)
	, database(nullptr)
	, indexer(nullptr)
{
}

//-----------------------------------//

PluginMetadata ResourcesPlugin::getMetadata()
{
	PluginMetadata metadata;
	metadata.name = "Resources";
	metadata.description = "Provides resources services";
	return metadata;
}

//-----------------------------------//

void ResourcesPlugin::onPluginEnable()
{
	Allocator* alloc = AllocatorGetServer();
	TaskPool* taskPool = GetServer()->taskPool;

	// Initialize resources.
	ResourcesInitialize();
	resources = Allocate(ResourceManager, alloc);
	resources->setTaskPool(taskPool);
	resources->setArchive(archive);
	resources->setAsynchronousLoading(true);
	resources->setupResourceLoaders(ResourceLoaderGetType());

	indexer = Allocate(ResourceIndexer, alloc);

	database = Allocate(ResourceDatabase, alloc);
	database->setIndexer(indexer);

	// Create a resource archive and index it.
	archive = ArchiveCreateFromDirectory(alloc, Settings::ResourcesPath);
	indexer->addArchive(archive);

	// Set up resource web server.
	webContext = WebServerCreate(alloc);
	WebServerStart(webContext, Settings::ResourcesPort);

	LogInfo("Starting HTTP server on port %hu", Settings::ResourcesPort);

	WebRoute route;
	route.pattern = "/Resources/*";
	route.callback.Bind(this, &ResourcesPlugin::onResourceRequest);

	WebServerAddRoute(webContext, route);
}

//-----------------------------------//

void ResourcesPlugin::onPluginDisable()
{
	LogInfo("Stopping HTTP server");
	WebServerStop(webContext);
	Deallocate(webContext);

	Deallocate(indexer);
	Deallocate(database);
	Deallocate(resources);

	ResourcesDeinitialize();
}

//-----------------------------------//

void ResourcesPlugin::update()
{
	indexer->update();
}

//-----------------------------------//

void ResourcesPlugin::onResourceRequest(WebContext* context, WebRequest* req)
{
	LogDebug("Requesting resource");

	MemoryStream* ms = StreamCreateFromMemory( AllocatorGetThis(), 8192 );
	
	Serializer* serializer = SerializerCreateJSON( AllocatorGetThis() );
	serializer->stream = ms;
	
	SerializerSave(serializer, database);
	Deallocate(serializer);

	mg_write(req->conn, ms->buffer, ms->position);
	Deallocate(ms);
}

//-----------------------------------//

NAMESPACE_SERVER_END