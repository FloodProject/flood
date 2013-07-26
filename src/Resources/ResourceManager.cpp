/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Resources/API.h"
#include "Resources/ResourceManager.h"
#include "Resources/ResourceLoader.h"

#include "Core/Log.h"
#include "Core/Memory.h"
#include "Core/Containers/Hash.h"
#include "Core/Containers/MurmurHash.h"
#include "Core/Concurrency.h"
#include "Core/Stream.h"
#include "Core/Archive.h"
#include "Core/Utilities.h"
#include "Core/Serialization.h"

NAMESPACE_RESOURCES_BEGIN

//-----------------------------------//

static ResourceManager* gs_ResourcesManager = nullptr;
ResourceManager* GetResourceManager() { return gs_ResourcesManager; }

static Allocator* gs_ResourcesAllocator = nullptr;
Allocator* GetResourcesAllocator() { return gs_ResourcesAllocator; }

void ResourcesInitialize()
{
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

static const char* GetResourceGroupString(ResourceGroup group)
{
    switch(group)
    {
    case ResourceGroup::General: return "General";
    case ResourceGroup::Images: return "Images";
    case ResourceGroup::Meshes: return "Meshes";
    case ResourceGroup::Fonts: return "Fonts";
    case ResourceGroup::Shaders: return "Shaders";
    case ResourceGroup::Audio: return "Audio";
    case ResourceGroup::Scripts: return "Scripts";
    case ResourceGroup::Scenes: return "Scenes";
    case ResourceGroup::Materials: return "Materials";
    case ResourceGroup::Particles: return "Particles";
    }

    assert(0 && "Unreachable");
    return nullptr;
}

ResourceHandle ResourceHandleCreate(Resource* res)
{
	if( !gs_ResourceHandleManager ) return HandleInvalid;
	
	HandleId handle = HandleCreate(gs_ResourceHandleManager, res);
    LogDebug("ResourceHandleCreate: %s %lu %s",
        GetResourceGroupString(res->getResourceGroup()), handle,
        res->getPath().c_str());
	
	return handle;
}

static bool gs_RemoveResource = true;

void ResourceHandleDestroy(HandleId id)
{
	Resource* resource = (Resource*) ResourceHandleFind(id);

	if( gs_RemoveResource )
		gs_ResourcesManager->removeResource(resource);
	
	LogDebug("ResourceHandleDestroy: %lu", id);
	HandleDestroy(gs_ResourceHandleManager, id);
}

static HandleId ResourceHandleFind(const char* s)
{
	if( !gs_ResourcesManager ) return HandleInvalid;
	return gs_ResourcesManager->loadResource(s).getId();
}

static void ResourceHandleSerialize(
	ReflectionContext* context, ReflectionWalkType wt )
{
	Serializer* serializer = (Serializer*) context->userData;

	Resource* resource = (Resource*) context->object;
	
	context->valueContext.s = &resource->path;
	context->primitive = &PrimitiveGetBuiltins().p_string;
	context->walkPrimitive(context, wt);
}

//-----------------------------------//

ResourceEvent::ResourceEvent()
	: resource(nullptr)
	, oldResource(nullptr)
{
}

//-----------------------------------//

ResourceManager::ResourceManager()
	: taskPool(nullptr)
	, archive(nullptr)
	, handleManager(nullptr)
	, numResourcesQueuedLoad(0)
	, asynchronousLoading(true)
	, resources(*AllocatorGetHeap())
	, resourceLoaders(*AllocatorGetHeap())
	, resourceLoaderExts(*AllocatorGetHeap())
{
	handleManager = HandleCreateManager( GetResourcesAllocator() );

	if( !gs_ResourcesManager )
		gs_ResourcesManager = this;

	if( !gs_ResourceHandleManager )
		gs_ResourceHandleManager = handleManager;

	ReflectionHandleContext context;
	context.type = ReflectionGetType(Resource);
	context.handles = gs_ResourceHandleManager;
	context.serialize = ResourceHandleSerialize;
	context.deserialize = ResourceHandleFind;
	
	ReflectionHandleContextMap contextMap(*AllocatorGetHeap());
	ReflectionSetHandleContext(&contextMap, context);

	resourceFinishLoadMutex = MutexCreate( GetResourcesAllocator() );
	resourceFinishLoad = ConditionCreate( GetResourcesAllocator() );
}

//-----------------------------------//

ResourceManager::~ResourceManager()
{
	destroyHandles();
	hash::clear(resourceLoaders);
	hash::clear(resourceLoaderExts);

	ConditionDestroy(resourceFinishLoad);
	MutexDestroy(resourceFinishLoadMutex);
}

//-----------------------------------//

void ResourceManager::destroyHandles()
{
	for(auto it : resources)
	{
		ResourceHandle& handle = it.value;
		Resource* res = handle.Resolve();
		
		LogDebug("Resource %s (refs: %d)", res->getPath().c_str(),
			res->references);
	}

	gs_RemoveResource = false;
	hash::clear(resources);
	
	HandleDestroyManager(handleManager);
	handleManager = nullptr;
}

//-----------------------------------//

ResourceHandle ResourceManager::getResource(const String& path)
{
	Path name = PathGetFile(path);

	auto key = murmur_hash_64(path.c_str(), path.size(), 0);
	auto res = hash::get(resources, key, ResourceHandle(HandleInvalid));

	return res;
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

ResourceHandle ResourceManager::loadResource(ResourceLoadOptions& options)
{
	if( !archive ) return ResourceHandle(HandleInvalid);

	Path fileExt = PathGetFileExtension(options.name);
	
	// If the file has no extension, search for one with the same
	// name but with known resource loadable file extensions.

	if(fileExt.empty() && !findResource(options))
	{
		LogError("Could not find matching file for '%s'", options.name.c_str());
		return ResourceHandle(HandleInvalid);
	}

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
	auto key = murmur_hash_64(base.c_str(), base.size(), 0);
	hash::set(resources, key, handle);

	decodeResource(options);

	return handle;
}

//-----------------------------------//

bool ResourceManager::findResource(ResourceLoadOptions& options)
{
	Path& path = options.name;

	for(auto it : resourceLoaders)
	{
		auto loader = it.value.get();
		auto ext = hash::get<String*>(resourceLoaderExts, it.key, nullptr);
		assert(ext != nullptr);

		if( loader->getResourceGroup() != options.group )
			continue;

		Path newPath = StringFormat("%s.%s", path.c_str(), ext->c_str());

		if( ArchiveExistsFile(archive, newPath) )
		{
			path = PathNormalize(newPath);
			return true;
		}
	}

	return false;
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

	options.stream = stream;

	Resource* resource = loader->prepare(options);
	
	if( !resource )
	{
		LogError("Error preparing resource: '%s'", path.c_str());
		return nullptr;
	}

	resource->setStatus( ResourceStatus::Loading );
	resource->setPath( file );

	options.resource = resource;

	return resource;
}

//-----------------------------------//

void ResourceTaskRun(Task* task);

void ResourceManager::decodeResource( ResourceLoadOptions& options )
{
	Task* task = TaskCreate( GetResourcesAllocator() );
	
	ResourceLoadOptions* taskOptions = Allocate(GetResourcesAllocator(),
		ResourceLoadOptions);

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

	while( resourceEvents.try_pop_front(event) )
	{
		Path base = PathGetFile(event.resource->path);

		// Find the handle to the resource.
		auto key = murmur_hash_64(base.c_str(), base.size(), 0);
		auto res = hash::get(resources, key, ResourceHandle(HandleInvalid));
		if( res == HandleInvalid )
			continue;

		event.handle = res;
		onResourceLoaded( event );
	}
}

//-----------------------------------//

void ResourceManager::removeUnusedResources()
{
	#pragma TODO("Finish the unused resource collecetor")

	return;

	Array<Resource*> resourcesToRemove(*AllocatorGetHeap());

	// Search for unused resources
	for( auto it : resources )
	{
		auto res = it.value.Resolve();

		if( res->references == 1 )
			resourcesToRemove.push_back(res);
	}

	for(auto res : resourcesToRemove)
		removeResource(res);
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
	auto key = murmur_hash_64(path.c_str(), path.size(), 0);
	auto res = hash::get(resources, key, ResourceHandle(HandleInvalid));
	if(res == HandleInvalid)
		return;
	
	// Send callback notifications.
	ResourceEvent event;
	event.handle = res;

	onResourceRemoved( event );

	LogInfo("Unloaded resource: %s", path.c_str());
	hash::remove(resources, key);
}

//-----------------------------------//

void ResourceManager::registerLoader(ResourceLoader* loader)
{
	if( !loader ) return;

	Class* klass = loader->getType();
	LogInfo( "Registering resource loader '%s'", klass->name );

	auto& extensions = loader->getExtensions();
	
	for( size_t i = 0; i < extensions.size(); ++i )
	{
		const String& extension = *extensions[i];
		auto key = murmur_hash_64(extension.c_str(), extension.size(), 0);

		if(hash::has(resourceLoaders, key))
		{
			LogDebug("Extension '%s' already has a resource loader",
				extension.c_str());
			continue;
		}

		hash::set(resourceLoaders, key, ResourceLoaderPtr(loader));
		hash::set(resourceLoaderExts, key, extensions[i]);
	}

	// Send callback notifications.
	onResourceLoaderRegistered( *loader );
}

//-----------------------------------//

ResourceLoader* ResourceManager::findLoader(const String& ext)
{
	String extension = StringToLowerCase(ext);
	auto key = murmur_hash_64(extension.c_str(), extension.size(), 0);
	auto loader = hash::get<ResourceLoaderPtr>(resourceLoaders, key, nullptr);

	return loader.get();
}

//-----------------------------------//

ResourceLoader* ResourceManager::findLoaderByClass(const Class* klass)
{
	for(auto it : resourceLoaders)
	{
		auto loader = it.value;
		Class* resourceClass = loader->getResourceClass();
		
		if(ClassInherits(resourceClass, klass))
			return loader.get();
	}

	return nullptr;
}

//-----------------------------------//

void ResourceManager::setupResourceLoaders(Class* klass)
{
	for( size_t i = 0; i < klass->childs.size(); ++i )
	{
		Class* child = klass->childs[i];

		if( !child->childs.empty() )
			setupResourceLoaders(child);
	
		if( ClassIsAbstract(child ) ) continue;

		auto loader = (ResourceLoader*) ClassCreateInstance(
			child, GetResourcesAllocator());

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
	auto key = murmur_hash_64(file.c_str(), file.size(), 0);
	auto res = hash::get(resources, key, ResourceHandle(HandleInvalid));

	if(res == HandleInvalid)
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

	Resource* oldResource = res.Resolve();
	HandleId handleId = res.getId();

	ResourceEvent event;
	event.resource = resource;
	event.oldResource = oldResource;
	event.handle = res;

	// Switch the resource but mantain the same handle.
	resource->addReference();
	hash::set<ReferenceCounted*>(handleManager->handles, (uint64)handleId, resource);

	onResourceReloaded(event);

	event.handle.id = HandleInvalid;
}

//-----------------------------------//

NAMESPACE_RESOURCES_END