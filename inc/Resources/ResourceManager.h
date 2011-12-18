/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Core/Concurrency.h"
#include "Resources/Resource.h"
#include "Resources/ResourceLoader.h"

NAMESPACE_RESOURCES_BEGIN

//-----------------------------------//

struct Stream;
struct Archive;
struct TaskPool;
struct FileWatchEvent;

class ResourceTask;
class ResourceManager;

FWD_DECL_INTRUSIVE(ResourceLoader)

//-----------------------------------//

/**
 * Event fired whenever an operation on the resource happens.
 * This can be useful to know when monitoring for changes,
 * for example in editors.
 */

struct ResourceEvent
{
	ResourceEvent();

	Resource* resource;
	Resource* oldResource;
	ResourceHandle handle;
};

//-----------------------------------//

API_RESOURCE void ResourcesInitialize();
API_RESOURCE void ResourcesDeinitialize();

// Gets the resource manager instance.
ResourceManager* GetResourceManager();

typedef std::map< String, ResourceHandle > ResourceMap;
typedef std::map< String, ResourceLoaderPtr > ResourceLoaderMap;

/**
 * Responsible for managing a set of resources that are added by the app.
 * It should be possible to enforce a strict memory budget, and the manager
 * will automatically load or unload the resources from memory, using for 
 * example an LRU policy.
 *
 * Each resource is mapped by an extension to a specific resource handler.
 * Various resource handlers can be registered to the same type and the one
 * found first the one used to handle the given resource. In the future the
 * resource matching could be extended to work with magic format signatures
 * which should prove to be less error-prone in case of a corrupt resource.
 */

class API_RESOURCE ResourceManager
{
	friend void ResourceTaskRun(Task* task);

public:

	ResourceManager();
	virtual ~ResourceManager();
 
	// Gets an already loaded resource by its name.
	ResourceHandle getResource(const Path& name);

	// Loads or returns an already loaded resource by its name.
	ResourceHandle loadResource(const Path& name);

	// Loads or returns an already loaded resource by its name.
	ResourceHandle loadResource(ResourceLoadOptions& options);

	// Removes a resource from the manager.
	void removeResource(Resource* resource);

	// Removes a resource from the manager.
	void removeResource(const String& name);

	// Removes unused resources.
	void removeUnusedResources();

	// Waits until all queued resources are loaded.
	void loadQueuedResources();

	// Finds the loader for the given extension.
	ResourceLoader* findLoader(const String& extension);

	// Finds the loader for the given type.
	ResourceLoader* findLoaderByClass(const Class* klass);

	// Sends resource events to the subscribers.
	void update();

	// Gets the registered resources.
	GETTER(Resources, const ResourceMap&, resources)

	// Gets the registered resource loaders.
	GETTER(ResourceLoaders, const ResourceLoaderMap&, resourceLoaders)

	// Gets/sets the threading status.
	ACESSOR(AsynchronousLoading, bool, asynchronousLoading)

	// Gets the handle manager.
	GETTER(HandleManager, HandleManager*, handleManager)

	// Accesses the task manager.
	ACESSOR(TaskPool, TaskPool*, taskPool)

	// Gets the archive.
	GETTER(Archive, Archive*, archive)

	// Sets the archive.
	void setArchive(Archive* archive);

	// These events are sent when their correspending actions happen.
	Event1< const ResourceEvent& > onResourcePrepared;
	Event1< const ResourceEvent& > onResourceLoaded;
	Event1< const ResourceEvent& > onResourceRemoved;
	Event1< const ResourceEvent& > onResourceReloaded;
	Event1< const ResourceLoader&> onResourceLoaderRegistered;

	// Gets an already loaded resource by its name.
	template <typename T>
	RESOURCE_HANDLE_TYPE(T) getResource(const String& name)
	{
		ResourceHandle res = getResource(name);
		return HandleCast<T>(res);
	}

	// Creates a new resource and returns the specific resource type.
	template <typename T>
	RESOURCE_HANDLE_TYPE(T) loadResource(const String& name)
	{
		ResourceHandle res = loadResource(name);
		return HandleCast<T>(res);
	}

	template <typename T>
	RESOURCE_HANDLE_TYPE(T) createResource()
	{
		ResourceHandle res = ResourceHandleCreate(AllocateHeap(T));
		return HandleCast<T>(res);
		
	}

	// Sets up the default resource loaders.
	void setupResourceLoaders(Class* klass);

	// Finds the true resource if needed.
	void findResource( ResourceLoadOptions& options );

protected:

	// Validates if the resource exists and if there is a loader for it.
	bool validateResource( const Path& path );

	// Returns a new resource ready to be processed by a loader.
	Resource* prepareResource( ResourceLoadOptions& options );

	// Processes the resource with the right resource loader.
	void decodeResource( ResourceLoadOptions& options );

	// Watches a resource for changes and auto-reloads it.
	void handleWatchResource(Archive*, const FileWatchEvent& event);

	// Sends pending resource events.
	void sendPendingEvents();

	// Destroy the resource handles.
	void destroyHandles();

	// Registers a resource handler.
	void registerLoader(const ResourceLoaderPtr&);

	// Maps a name to a resource.
	ResourceMap resources;

	// Maps extensions to resource loaders.
	ResourceLoaderMap resourceLoaders;

	// When tasks finish, they queue an event.
	ConcurrentQueue<ResourceEvent> resourceTaskEvents;

	// Keeps track if asynchronous loading is enabled.
	bool asynchronousLoading;

	Archive* archive;
	TaskPool* taskPool;
	HandleManager* handleManager;

	Condition* resourceFinishLoad;
	Mutex* resourceFinishLoadMutex;

	// Number of resources queued for loading.
	volatile Atomic numResourcesQueuedLoad;
};

//-----------------------------------//

NAMESPACE_RESOURCES_END