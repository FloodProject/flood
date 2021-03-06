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
        res->getPath().CString());
	
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
	
    context->valueContext.us = &resource->path;
    context->primitive = &PrimitiveBuiltins::GetBuiltins().p_utf8string;
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
{
	handleManager = HandleCreateManager( GetResourcesAllocator() );

	if( !gs_ResourcesManager )
		gs_ResourcesManager = this;

	if( !gs_ResourceHandleManager )
		gs_ResourceHandleManager = handleManager;

	ReflectionHandleContext context;
	context.type = ResourceGetType();
	context.handles = gs_ResourceHandleManager;
	context.serialize = ResourceHandleSerialize;
	context.deserialize = ResourceHandleFind;
	
	ReflectionHandleContextMap contextMap;
	ReflectionSetHandleContext(&contextMap, context);

	resourceFinishLoadMutex = Allocate(GetResourcesAllocator(), Mutex);
	resourceFinishLoad = Allocate(GetResourcesAllocator(), Condition);
}

//-----------------------------------//

ResourceManager::~ResourceManager()
{
	destroyHandles();
    resourceLoaders.Clear();

	Deallocate(resourceFinishLoad);
	Deallocate(resourceFinishLoadMutex);
}

//-----------------------------------//

void ResourceManager::destroyHandles()
{
    for( auto it = resources.Begin(); it != resources.End(); ++it )
	{
		ResourceHandle& handle = it->second;
		Resource* res = handle.Resolve();
		
        LogDebug("Resource %s (refs: %d)", res->getPath().CString(),
			res->references.read());
	}

	gs_RemoveResource = false;
    resources.Clear();
	
	HandleDestroyManager(handleManager);
	handleManager = nullptr;
}

//-----------------------------------//

ResourceHandle ResourceManager::getResource(const Path& path)
{
	Path name = PathGetFile(path);

    if( resources.Find(name) == resources.End() )
		return ResourceHandle(HandleInvalid);

	return resources[name];
}

//-----------------------------------//

ResourceHandle ResourceManager::loadResource(const Path& path)
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

    if(fileExt.Empty() && !findResource(options))
	{
        LogError("Could not find matching file for '%s'", options.name.CString());
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
	resources[base] = handle;

	decodeResource(options);

	return handle;
}

//-----------------------------------//

bool ResourceManager::findResource(ResourceLoadOptions& options)
{
	Path& path = options.name;
	
    ResourceLoaderMap::ConstIterator it;
    for(it = resourceLoaders.Begin(); it != resourceLoaders.End(); it++)
	{
		const String& ext = it->first;
		const ResourceLoader* loader = it->second.get();

		if( loader->getResourceGroup() != options.group )
			continue;

        Path newPath = StringFormat("%s.%s", path.CString(), ext.CString());

		if (archive->existsFile(newPath))
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
    if( path.Empty() ) return false;
	
	const Path& extension = PathGetFileExtension(path);
	
    if( extension.Empty() )
	{
        LogWarn( "Resource '%s' has an invalid extension", path.CString() );
		return false;
	}

	if( !findLoader(extension) )
	{
        LogWarn("No resource loader found for resource '%s'", path.CString());
		return false;
	}

	return true;
}

//-----------------------------------//

Resource* ResourceManager::prepareResource( ResourceLoadOptions& options )
{
	const Path& path = options.name;

	Stream* stream = archive->openFile(path, GetResourcesAllocator());
	
	if( !stream )
	{
        LogWarn("Resource was not found: '%s'", path.CString());
		return nullptr;
	}

	const Path& file = PathGetFile(path);

	// Get the available resource loader and prepare the resource.
	ResourceLoader* loader = findLoader( PathGetFileExtension(file) );

	if( !loader )
	{
        LogWarn("No resource loader found for resource '%s'", file.CString());
		return nullptr;
	}

	options.stream = stream;

	Resource* resource = loader->prepare(options);
	
	if( !resource )
	{
        LogError("Error preparing resource: '%s'", path.CString());
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
	Task* task = Allocate(GetResourcesAllocator(), Task);
	
	ResourceLoadOptions* taskOptions = Allocate(GetResourcesAllocator(),
		ResourceLoadOptions);

	*taskOptions = options;

	task->callback.Bind(ResourceTaskRun);
	task->userdata = taskOptions;

	numResourcesQueuedLoad.increment();

#ifdef ENABLE_THREADED_LOADING
	if( taskPool && asynchronousLoading && options.asynchronousLoad )
	{
		taskPool->add(task, options.isHighPriority);
		return;
	}
#endif

	task->run();
	sendPendingEvents();
}

//-----------------------------------//

void ResourceManager::loadQueuedResources()
{
	resourceFinishLoadMutex->lock();

	while(numResourcesQueuedLoad.read() > 0 )
	{
		#pragma TODO("Use a sleep and notify the observers of progress")
		resourceFinishLoad->wait(*resourceFinishLoadMutex);
	}

	resourceFinishLoadMutex->unlock();

	assert(numResourcesQueuedLoad.read() == 0);
}

//-----------------------------------//

void ResourceManager::update()
{
	sendPendingEvents();

	// Update the archive watches.
	archive->monitor();

	removeUnusedResources();
}

//-----------------------------------//

void ResourceManager::sendPendingEvents()
{
	ResourceEvent event;

	while( resourceEvents.try_pop_front(event) )
	{
		Resource* resource = event.resource;
		Path base = PathGetFile(resource->path);

		// Find the handle to the resource.
        ResourceMap::Iterator it = resources.Find(base);
        if( it == resources.End() ) continue;

		ResourceHandle handle = it->second;
		assert( handle != HandleInvalid );

		event.handle = handle;
		onResourceLoaded( event );
	}
}

//-----------------------------------//

void ResourceManager::removeUnusedResources()
{
	#pragma TODO("Finish the unused resource collecetor")

	return;

    Vector<String> resourcesToRemove;

	// Search for unused resources.
    ResourceMap::ConstIterator it;
    for( it = resources.Begin(); it != resources.End(); it++ )
	{
		const ResourceHandle& resource = it->second;

		if( resource.Resolve()->references.read() == 1 )
            resourcesToRemove.Push(it->first);
	}

    for( size_t i = 0; i < resourcesToRemove.Size(); i++ )
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
    ResourceMap::Iterator it = resources.Find(path);
	
    if( it == resources.End() )
		return;
	
	// Send callback notifications.
	ResourceEvent event;
	event.handle = it->second;

	onResourceRemoved( event );

    LogInfo("Unloaded resource: %s", path.CString());
    resources.Erase(it);
}

//-----------------------------------//

void ResourceManager::registerLoader(ResourceLoader* loader)
{
	if( !loader ) return;

	Class* klass = loader->getType();
	LogInfo( "Registering resource loader '%s'", klass->name );

    const Vector<String>& extensions = loader->getExtensions();
	
    for( size_t i = 0; i < extensions.Size(); i++ )
	{
		const String& extension = extensions[i];

        if(resourceLoaders.Find(extension) != resourceLoaders.End())
		{
			LogDebug("Extension '%s' already has a resource loader",
                extension.CString());
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
    String extension = ext.ToLower();

	// Check if we have a resource loader for this extension.
    if( resourceLoaders.Find(extension) == resourceLoaders.End() )
		return nullptr;

	const ResourceLoaderPtr& loader = resourceLoaders[extension];
	return loader.get();
}

//-----------------------------------//

ResourceLoader* ResourceManager::findLoaderByClass(const Class* klass)
{
    for(auto it = resourceLoaders.Begin(); it != resourceLoaders.End(); it++)
	{
		const ResourceLoaderPtr& loader = it->second;
		Class* resourceClass = loader->getResourceClass();
		
		if(resourceClass->inherits(klass))
			return loader.get();
	}

	return nullptr;
}

//-----------------------------------//

void ResourceManager::setupResourceLoaders(Class* klass)
{
	for(auto& child : klass->childs)
	{
		if( !child->childs.Empty() )
			setupResourceLoaders(child);
	
		if (child->isAbstract()) continue;

		auto loader = (ResourceLoader*) 
			child->createInstance(GetResourcesAllocator());

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

    if( resources.Find(file) == resources.End() )
		return; // Resource is not known.

	// Reload the resource if it was modified.
	if( evt.action != FileWatchEventKind::Modified )
	{
		#pragma TODO("Add rename support in live updating")

		LogDebug( "Resource was renamed - handle this" );
		return;
	}

	// Register the decoded resource in the map.
    LogInfo("Reloading resource '%s'", file.CString());

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