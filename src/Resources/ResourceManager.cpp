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

NAMESPACE_RESOURCES_BEGIN

//-----------------------------------//

static ResourceManager* gs_ResourcesManager = nullptr;;
ResourceManager* GetResourceManager() { return gs_ResourcesManager; }

static Allocator* gs_ResourcesAllocator = nullptr;
Allocator* GetResourcesAllocator() { return gs_ResourcesAllocator; }

extern void ReferenceLoaders();

void ResourcesInitialize()
{
	ReferenceLoaders();

	gs_ResourcesAllocator = AllocatorCreateHeap( AllocatorGetHeap() );
	AllocatorSetGroup(gs_ResourcesAllocator, "Resources");
}

void ResourcesDeinitialize()
{
	AllocatorDestroy(gs_ResourcesAllocator);
}

//-----------------------------------//

static HandleManager* gs_ResourceHandleManager = nullptr;

ReferenceCounted* ResourceHandleFind(HandleId id)
{
	if( !gs_ResourceHandleManager ) return nullptr;

	Resource* res = (Resource*) HandleFind(gs_ResourceHandleManager, id);
	return res;
}

ResourceHandle ResourceHandleCreate(Resource* p)
{
	if( !gs_ResourceHandleManager ) return HandleInvalid;
	
	HandleId handle = HandleCreate(gs_ResourceHandleManager, p);
	LogDebug("ResourceHandleCreate: %lu %s", handle, p->getPath().c_str());
	
	return handle;
}

void ResourceHandleDestroy(HandleId id)
{
	Resource* resource = (Resource*) ResourceHandleFind(id);
	//gs_ResourcesManager->removeResource(resource);
	
	LogDebug("ResourceHandleDestroy: %lu", id);
	HandleDestroy(gs_ResourceHandleManager, id);

	Deallocate(resource);
}

static HandleId ResourceHandleFind(const char* s)
{
	if( !gs_ResourcesManager ) return HandleInvalid;
	return gs_ResourcesManager->loadResource(s).getId();
}

static void ResourceHandleSerialize( ReflectionContext* context, ReflectionWalkType::Enum wt )
{
	Serializer* serializer = (Serializer*) context->userData;

	Resource* resource = (Resource*) context->object;
	
	context->valueContext.s = &resource->path;
	context->primitive = &Primitive::s_string;
	context->walkPrimitive(context, wt);
}

//-----------------------------------//

ResourceLoadOptions::ResourceLoadOptions()
	: group(ResourceGroup::General)
	, asynchronousLoad(true)
	, sendLoadEvent(true)
	, isHighPriority(false)
	, stream(nullptr)
	, resource(nullptr)
{
}

//-----------------------------------//

ResourceManager::ResourceManager()
	: taskPool(nullptr)
	, archive(nullptr)
	, handleManager(nullptr)
	, numResourcesQueuedLoad(0)
	, asynchronousLoading(true)
{
	handleManager = HandleCreateManager( GetResourcesAllocator() );

	if( !gs_ResourcesManager ) gs_ResourcesManager = this;
	if( !gs_ResourceHandleManager ) gs_ResourceHandleManager = handleManager;

	ReflectionHandleContext context;
	context.type = ReflectionGetType(Resource);
	context.handles = gs_ResourceHandleManager;
	context.serialize = ResourceHandleSerialize;
	context.deserialize = ResourceHandleFind;
	
	ReflectionSetHandleContext(context);

	resourceFinishLoadMutex = MutexCreate( GetResourcesAllocator() );
	resourceFinishLoad = ConditionCreate( GetResourcesAllocator() );
}

//-----------------------------------//

ResourceManager::~ResourceManager()
{
	destroyHandles();
	resourceLoaders.clear();

	ArchiveDestroy(archive);
	ConditionDestroy(resourceFinishLoad);
	MutexDestroy(resourceFinishLoadMutex);
}

//-----------------------------------//

void ResourceManager::destroyHandles()
{
	ResourceMap::iterator it = resources.begin();
	for( ; it != resources.end(); ++it )
	{
		ResourceHandle& handle = it->second;
		Resource* res = handle.Resolve();
		
		LogDebug("Resource %s (refs: %d)", res->getPath().c_str(), res->references);
	}

	resources.clear();
	
	HandleDestroyManager(handleManager);
	handleManager = nullptr;
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

ResourceHandle ResourceManager::loadResource(const String& path)
{
	Path name = PathGetFile(path);

	ResourceLoadOptions options;
	options.name = name;
	options.asynchronousLoad = asynchronousLoading;
	
	return loadResource(options);
}

//-----------------------------------//

ResourceHandle ResourceManager::loadResource(ResourceLoadOptions options)
{
	if( !archive ) return ResourceHandle(HandleInvalid);

	findResource(options);

	// Check if the resource is already loaded.
	ResourceHandle handle = getResource(options.name);
	if( handle ) return handle;

	if( !validateResource(options.name) )
		return ResourceHandle(HandleInvalid);

	Resource* resource = prepareResource(options);
	
	if( !resource )
		return ResourceHandle(HandleInvalid); 

	handle = ResourceHandleCreate(resource);
	
	if(handle == HandleInvalid)
		return ResourceHandle(HandleInvalid);

	// Register the decoded resource in the map.
	Path base = PathGetFile(options.name);
	resources[base] = handle;

	decodeResource(options);

	return handle;
}

//-----------------------------------//

void ResourceManager::findResource(ResourceLoadOptions& options)
{
	Path& path = options.name;
	
	const Path& ext = PathGetFileExtension(path);
	if( !ext.empty() ) return;

	ResourceLoaderMap::const_iterator it;
	for(it = resourceLoaders.begin(); it != resourceLoaders.end(); it++)
	{
		const String& ext = it->first;
		const ResourceLoaderPtr& loader = it->second;

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

bool ResourceManager::validateResource( const Path& path )
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

Resource* ResourceManager::prepareResource( ResourceLoadOptions& options )
{
	const Path& path = options.name;

	Stream* stream = ArchiveOpenFile(archive, path, GetResourcesAllocator());
	
	if( !stream )
	{
		LogWarn("Resource was not found: '%s'", path.c_str());
		return nullptr;
	}

	const Path& file = PathGetFile(path);

	// Get the available resource loader and prepare the resource.
	ResourceLoader* loader = findLoader( PathGetFileExtension(file) );

	if( !loader )
	{
		LogWarn("No resource loader found for resource '%s'", file.c_str());
		return nullptr;
	}

	Resource* resource = loader->prepare(*stream);
	resource->setStatus( ResourceStatus::Loading );
	resource->setPath( file );

	options.stream = stream;
	options.resource = resource;

	return resource;
}

//-----------------------------------//

void ResourceTaskRun(Task* task);

void ResourceManager::decodeResource( ResourceLoadOptions& options )
{
	Task* task = TaskCreate( GetResourcesAllocator() );
	
	ResourceLoadOptions* taskOptions = Allocate(ResourceLoadOptions,  GetResourcesAllocator());
	*taskOptions = options;

	task->callback.Bind(ResourceTaskRun);
	task->userdata = taskOptions;

	AtomicIncrement(&numResourcesQueuedLoad);

#ifdef ENABLE_THREADED_LOADING
	if( taskPool && asynchronousLoading && options.asynchronousLoad )
	{
		TaskPoolAdd(taskPool, task, options.isHighPriority);
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
		#pragma TODO("Use a sleep and notify the observers of progress")
		ConditionWait(resourceFinishLoad, resourceFinishLoadMutex);
	}

	MutexUnlock(resourceFinishLoadMutex);

	assert( AtomicRead(&numResourcesQueuedLoad) == 0 );
}

//-----------------------------------//

void ResourceManager::update()
{
	sendPendingEvents();

	// Update the archive watches.
	ArchiveWatchUpdate(archive);

	removeUnusedResources();
}

//-----------------------------------//

void ResourceManager::sendPendingEvents()
{
	ResourceEvent event;

	while( resourceTaskEvents.try_pop_front(event) )
	{
		Resource* resource = event.resource;
		Path base = PathGetFile(resource->path);

		// Find the handle to the resource.
		ResourceMap::iterator it = resources.find(base);
		if( it == resources.end() ) continue;

		ResourceHandle handle = it->second;
		assert( handle != HandleInvalid );

		event.handle = handle;
		onResourceLoaded( event );
	}
}

//-----------------------------------//

void ResourceManager::removeUnusedResources()
{
	return;

	std::vector<String> resourcesToRemove;

	// Search for unused resources.
	ResourceMap::const_iterator it;
	for( it = resources.begin(); it != resources.end(); it++ )
	{
		const ResourceHandle& resource = it->second;

		if( resource.Resolve()->references == 1 )
			resourcesToRemove.push_back(it->first);
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

void ResourceManager::registerLoader(const ResourceLoaderPtr& loader)
{
	if( !loader ) return;

	Class* klass = loader->getType();
	LogInfo( "Registering resource loader '%s'", klass->name );

	const std::vector<String>& extensions = loader->getExtensions();
	
	for( size_t i = 0; i < extensions.size(); i++ )
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
	onResourceLoaderRegistered( *loader.get() );
}

//-----------------------------------//

ResourceLoader* ResourceManager::findLoader(const String& ext)
{
	// Check if we have a resource loader for this extension.
	if( resourceLoaders.find(ext) == resourceLoaders.end() )
		return nullptr;

	const ResourceLoaderPtr& loader = resourceLoaders[ext];
	return loader.get();
}

//-----------------------------------//

ResourceLoader* ResourceManager::findLoaderByClass(const Class* klass)
{
	for(auto it = resourceLoaders.begin(); it != resourceLoaders.end(); it++)
	{
		const ResourceLoaderPtr& loader = it->second;
		Class* resourceClass = loader->getResourceClass();
		
		if(ClassInherits(resourceClass, klass))
			return loader.get();
	}

	return nullptr;
}

//-----------------------------------//

void ResourceManager::setupResourceLoaders(Class* klass)
{
	for( size_t i = 0; i < klass->childs.size(); i++ )
	{
		Class* child = klass->childs[i];

		if( !child->childs.empty() )
			setupResourceLoaders(child);
	
		if( ClassIsAbstract(child ) ) continue;

		ResourceLoader* loader = (ResourceLoader*) ClassCreateInstance(child, GetResourcesAllocator());
		registerLoader( loader );
	}
}

//-----------------------------------//

void ResourceManager::setArchive(Archive* newArchive)
{
	if(archive == newArchive) return;

	if(archive)
	{
		// Disconnect from the watch events.
		archive->watch.Disconnect(this, &ResourceManager::handleWatchResource);
		archive = nullptr;
	}

	if(newArchive)
	{
		archive = newArchive;
		archive->watch.Connect(this, &ResourceManager::handleWatchResource);
	}
}

//-----------------------------------//

void ResourceManager::handleWatchResource(Archive*, const FileWatchEvent& evt)
{
	// Check if the filename maps to a known resource.
	const Path& file = PathGetFile(evt.filename);

	if( resources.find(file) == resources.end() )
		return; // Resource is not known.

	// Reload the resource if it was modified.
	if( evt.action != FileWatchEvent::Modified )
	{
		#pragma TODO("Add rename support in live updating")

		LogDebug( "Resource was renamed - handle this" );
		return;
	}

	// Register the decoded resource in the map.
	LogInfo("Reloading resource '%s'", file.c_str());

	ResourceLoadOptions options;
	options.sendLoadEvent = false;
	options.name = evt.filename;
	
	Resource* resource = prepareResource(options);
	decodeResource(options);

	const ResourceHandle& handle = resources[file];
	Resource* oldResource = handle.Resolve();

	HandleId handleId = handle.getId();

	ResourceEvent event;
	event.resource = resource;
	event.oldResource = oldResource;
	event.handle = handle;

	// Switch the resource but mantain the same handle.
	resource->addReference();
	handleManager->handles[handleId] = resource;

	onResourceReloaded(event);

	event.handle.id = HandleInvalid;
}

//-----------------------------------//

NAMESPACE_RESOURCES_END