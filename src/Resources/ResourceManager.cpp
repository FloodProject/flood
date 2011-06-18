/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#ifdef API_CORE_DLL
#define INSTANTIATE_TEMPLATES
#endif

#include "Resources/API.h"
#include "Resources/ResourceManager.h"
#include "Resources/ResourceLoader.h"

#include "Core/Log.h"
#include "Core/Memory.h"
#include "Core/Concurrency.h"
#include "Core/Stream.h"
#include "Core/Archive.h"
#include "Core/Utilities.h"
#include "Core/Serialization.h"

NAMESPACE_BEGIN

//-----------------------------------//

static Allocator* g_ResourcesAllocator;
static ResourceManager* g_ResourcesManager;
static HandleManager* g_ResourceHandleManager = nullptr;

ResourceManager* GetResourceManager() { return g_ResourcesManager; }
Allocator* GetResourcesAllocator() { return g_ResourcesAllocator; }

void* ResourceHandleFind(HandleId id)
{
	return HandleFind(g_ResourceHandleManager, id);
}

ResourceHandle ResourceHandleCreate(Resource* p)
{
	return HandleCreate(g_ResourceHandleManager, p);
}

void ResourceHandleDestroy(HandleId id)
{
	Resource* resource = (Resource*) ResourceHandleFind(id);
	//g_ResourcesManager->removeResource(resource);

	Deallocate(resource);
	HandleDestroy(g_ResourceHandleManager, id);
}

static HandleId ResourceFind(const char* s)
{
	return g_ResourcesManager->loadResource(s).id;
}

//-----------------------------------//

ResourceLoadOptions::ResourceLoadOptions()
	: group(ResourceGroup::General)
	, asynchronousLoad(true)
	, sendLoadEvent(true)
	, handle(HandleInvalid)
	, stream(nullptr)
{ }

//-----------------------------------//

ResourceManager::ResourceManager()
	: taskPool(nullptr)
	, archive(nullptr)
	, handleManager(nullptr)
	, numResourcesQueuedLoad(0)
	, asynchronousLoading(true)
{
	if( !g_ResourcesAllocator ) 
		g_ResourcesAllocator = AllocatorCreateHeap( AllocatorGetHeap(), "Resources" );

	handleManager = HandleCreateManager( GetResourcesAllocator() );

	if( !g_ResourcesManager ) g_ResourcesManager = this;
	if( !g_ResourceHandleManager ) g_ResourceHandleManager = handleManager;

	ReflectionHandleContext context;
	context.type = ReflectionGetType(Resource);
	context.handles = g_ResourceHandleManager;
	context.deserialize = ResourceFind;
	ReflectionSetHandleContext(context);

	resourceFinishLoadMutex = MutexCreate( GetResourcesAllocator() );
	resourceFinishLoad = ConditionCreate( GetResourcesAllocator() );
}

//-----------------------------------//

ResourceManager::~ResourceManager()
{
	loadQueuedResources();
	assert( resourceTaskEvents.empty() );

	destroyLoaders();

	resources.clear();

	ArchiveDestroy(archive);
	HandleDestroyManager(handleManager);

	ConditionDestroy(resourceFinishLoad);
	MutexDestroy(resourceFinishLoadMutex);
	AllocatorDestroy(GetResourcesAllocator());
}

//-----------------------------------//

void ResourceManager::destroyLoaders()
{
	ResourceLoaderMap::const_iterator it;
	
	for( it = resourceLoaders.begin(); it != resourceLoaders.end(); it++ )
	{
		ResourceLoader* loader = it->second;
		if( !loader ) continue;

		std::vector<String>& extensions = loader->extensions;

		if( extensions.size() == 1 )
			Deallocate(loader);
		else
			extensions.erase( std::find(extensions.begin(), extensions.end(), it->first) );
	}
}

//-----------------------------------//

ResourceHandle ResourceManager::getResource(const String& path)
{
	Path name = PathGetFile(path);

	if( resources.find(name) == resources.end() )
		return ResourceHandle(HandleInvalid);

	return resources[name];
}

//-----------------------------------//

ResourceHandle ResourceManager::loadResource(const String& name)
{
	ResourceLoadOptions options;
	options.name = name;
	options.asynchronousLoad = asynchronousLoading;
	
	return loadResource(options);
}

//-----------------------------------//

ResourceHandle ResourceManager::loadResource(ResourceLoadOptions options)
{
	if( !archive )
		return ResourceHandle(HandleInvalid);

	findResource(options);

	// Check if the resource is already loaded.
	ResourceHandle handle = getResource(options.name);
	
	if( handle )
		return handle;

	if( !validateResource(options.name) )
		return ResourceHandle(HandleInvalid);

	Stream* stream = ArchiveOpenFile(archive, options.name, GetResourcesAllocator());

	if( !stream )
		return ResourceHandle(HandleInvalid);

	handle = prepareResource(stream);
	
	if( !handle )
		return ResourceHandle(HandleInvalid);

	options.stream = stream;
	options.handle = handle;

	decodeResource(options);

	// Register the decoded resource in the map.

	Path base = PathGetFile(options.name);
	resources[base] = handle;

	return handle;
}

//-----------------------------------//

void ResourceManager::findResource(ResourceLoadOptions& options)
{
	const Path& fileExt = PathGetFileExtension(options.name);
	Path& path = options.name;

	if( !fileExt.empty() )
		return;

	ResourceLoaderMap::const_iterator it;
	for(it = resourceLoaders.begin(); it != resourceLoaders.end(); it++)
	{
		const String& ext = it->first;
		ResourceLoader* loader = it->second;

		if( loader->getResourceGroup() != options.group )
			continue;

		Path newPath = StringFormat("%s.%s", path.c_str(), ext.c_str());

		if( ArchiveExistsFile(archive, newPath) )
		{
			path = PathNormalize(newPath);
			break;
		} 
	}
}

//-----------------------------------//

bool ResourceManager::validateResource( const String& path )
{
	if( path.empty() ) return false;
	
	const Path& extension = PathGetFileExtension(path);
	
	if( extension.empty() )
	{
		LogWarn( "Resource '%s' has an invalid extension", path.c_str() );
		return false;
	}

	if( !findLoader(extension) )
	{
		LogWarn("No resource loader found for resource '%s'", path.c_str());
		return false;
	}

	return true;
}

//-----------------------------------//

ResourceHandle ResourceManager::prepareResource(Stream* stream)
{
	const Path& path = PathGetFile(stream->path);

	// Get the available resource loader and prepare the resource.
	ResourceLoader* loader = findLoader( PathGetFileExtension(path) );

	if( !loader )
	{
		LogWarn("No resource loader found for resource '%s'", path.c_str());
		return ResourceHandle(HandleInvalid);
	}

	Resource* resource = loader->prepare(*stream);
	resource->setStatus( ResourceStatus::Loading );
	resource->setPath( path );

	ResourceHandle handle = HandleCreate(handleManager, resource);

	// Send callback notifications.
	ResourceEvent event;
	event.handle = handle;

	onResourcePrepared(event);

	return handle;
}

//-----------------------------------//

void ResourceManager::decodeResource( ResourceLoadOptions& options )
{
	Task* task = TaskCreate( GetResourcesAllocator() );
	
	ResourceLoadOptions* taskOptions = Allocate(ResourceLoadOptions,  GetResourcesAllocator());
	*taskOptions = options;

	task->Callback.Bind(ResourceTaskRun);
	task->Userdata = taskOptions;

	AtomicIncrement(&numResourcesQueuedLoad);;

#ifdef ENABLE_THREADED_LOADING
	if( taskPool && options.asynchronousLoad )
	{
		TaskPoolAdd(taskPool, task);
		return;
	}
#endif

	TaskRun(task);
	sendPendingEvents();
}

//-----------------------------------//

void ResourceManager::loadQueuedResources()
{
	MutexLock(resourceFinishLoadMutex);

	while( AtomicRead(&numResourcesQueuedLoad) > 0 )
	{
		#pragma TODO("Use timed_wait and notify the observers of progress")

		ConditionWait(resourceFinishLoad, resourceFinishLoadMutex);
		//SystemSleep( 0.01f );
	}

	MutexUnlock(resourceFinishLoadMutex);

	assert( AtomicRead(&numResourcesQueuedLoad) == 0 );
}

//-----------------------------------//

void ResourceManager::update( float )
{
	sendPendingEvents();
	removeUnusedResources();
}

//-----------------------------------//

void ResourceManager::sendPendingEvents()
{
	// Send resource events.
	ResourceEvent event;

	while( resourceTaskEvents.try_pop(event) )
	{
		onResourceLoaded( event );	
	}
}

//-----------------------------------//

void ResourceManager::removeUnusedResources()
{
	std::vector<String> resourcesToRemove;

	// Search for unused resources.
	ResourceMap::const_iterator it;
	for( it = resources.begin(); it != resources.end(); it++ )
	{
#if 0
		const ResourcePtr& resource = it->second;

		if( resource->getReferenceCount() == 1 )
			resourcesToRemove.push_back(it->first);
#endif
	}

	for( size_t i = 0; i < resourcesToRemove.size(); i++ )
	{
		const String& resource = resourcesToRemove[i];
		removeResource(resource);
	}
}

//-----------------------------------//

void ResourceManager::removeResource(Resource* resource)
{
	if( !resource ) return;

	const String& path = resource->getPath();
	removeResource(path);
}

//-----------------------------------//

void ResourceManager::removeResource(const String& path)
{
	ResourceMap::iterator it = resources.find(path);
	
	if( it == resources.end() )
		return;
	
	// Send callback notifications.
	ResourceEvent event;
	event.handle = it->second;
		
	onResourceRemoved( event );

	LogInfo("Unloaded resource: %s", path.c_str());
	resources.erase(it);
}

//-----------------------------------//

void ResourceManager::registerLoader(ResourceLoader* const loader)
{
	LogInfo( "Registering resource loader '%s'", loader->getName().c_str() );

	const std::vector<String>& extensions = loader->getExtensions();
	
	for( uint i = 0; i < extensions.size(); i++ )
	{
		const String& extension = extensions[i];

		if(resourceLoaders.find(extension) != resourceLoaders.end())
		{
			LogDebug("Extension '%s' already has a resource loader", extension.c_str());
			continue;
		}

		resourceLoaders[extension] = loader;
	}

	// Send callback notifications.
	onResourceLoaderRegistered( *loader );
}

//-----------------------------------//

ResourceLoader* ResourceManager::findLoader(const String& ext)
{
	// Check if we have a resource loader for this extension.
	if( resourceLoaders.find(ext) == resourceLoaders.end() )
		return nullptr;

	ResourceLoader* loader = resourceLoaders[ext];
	return loader;
}

//-----------------------------------//

void ResourceManager::setupResourceLoaders()
{
	Class* klass = ResourceLoaderGetType();
	
	for( size_t i = 0; i < klass->childs.size(); i++ )
	{
		Class* child = klass->childs[i];
	
		ResourceLoader* loader = (ResourceLoader*) ClassCreateInstance(child, GetResourcesAllocator());
		registerLoader( loader );
	}

	// Dummy call so linker doesn't strip the loaders.
	referenceLoaders();
}

//-----------------------------------//

void ResourceManager::handleWatchResource(const FileWatchEvent& evt)
{
	/*
	// Check if the filename maps to a known resource.
	const String& file = evt.filename;

	if( resources.find(file) == resources.end() )
		return; // Resource is not known.

	const ResourcePtr& res = resources[file];

	// Reload the resource if it was modified.
	if( evt.action != Actions::Modified )
	{
		#pragma TODO("Add rename support in live updating")

		LogDebug( "Resource was renamed - handle this" );
		return;
	}

	// Register the decoded resource in the map.
	LogInfo("Reloading resource '%s'", file.c_str());

	ResourceLoadOptions options;
	options.sendLoadEvent = false;

	decodeResource( res, options );

	ResourceEvent event;
	event.resource = res;
	
	onResourceReloaded(event);
	*/
}

//-----------------------------------//

NAMESPACE_END