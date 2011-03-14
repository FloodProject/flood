/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Subsystem.h"
#include "ConcurrentQueue.h"
#include "ReferenceCount.h"
#include "Resources/Resource.h"

namespace vapor {

//-----------------------------------//

struct File;
class FileWatcher;
class FileWatchEvent;

struct TaskPool;
class ResourceTask;
class ResourceLoader;

//-----------------------------------//

/**
 * Event fired whenever an operation on the resource happens.
 * This can be useful to know when monitoring for changes,
 * for example in editors.
 */

struct ResourceEvent
{
	ResourcePtr resource;
	ResourceLoader* loader;
};

struct ResourceLoadOptions
{
	ResourceLoadOptions();

	std::string name;
	ResourceGroup::Enum group;
	bool asynchronousLoading;
	bool sendLoadEvent;
	Resource* resource;
};

//-----------------------------------//

typedef std::map< std::string, ResourcePtr > ResourceMap;
typedef std::map< std::string, ResourceLoader* > ResourceLoaderMap;

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



class RESOURCE_API ResourceManager
{
	friend void ResourceTaskRun(Task* task);

public:

	ResourceManager();
	virtual ~ResourceManager();
 
	// Gets an already loaded resource by its name.
	ResourcePtr getResource(const std::string& name);

	// Loads or returns an already loaded resource by its name.
	ResourcePtr loadResource(const std::string& name);

	// Loads or returns an already loaded resource by its name.
	ResourcePtr loadResource(ResourceLoadOptions options);

	// Removes a resource from the manager.
	void removeResource(const ResourcePtr& res);

	// Removes a resource from the manager.
	void removeResource(const std::string& path);

	// Removes unused resources.
	void removeUnusedResources();

	// Finishes until all queued resources are loaded.
	void loadQueuedResources();

	// Registers a resource handler.
	void registerLoader(ResourceLoader* const loader);

	// Finds the loader for the given extension.
	ResourceLoader* findLoader(const std::string& extension);

	// Sets up the default resource loaders.
	void setupResourceLoaders();

	// Sends resource events to the subscribers.
	void update( float );

	// Gets the registered resources.
	GETTER(Resources, const ResourceMap&, resources)

	// Gets the registered resource loaders.
	GETTER(ResourceLoaders, const ResourceLoaderMap&, resourceLoaders)

	// Gets/sets the threading status.
	ACESSOR(AsynchronousLoading, bool, asynchronousLoading)

	// Sets the task manager.
	SETTER(TaskPool, TaskPool*, taskPool) 

	// Sets the file watcher.
	void setFileWatcher(FileWatcher* watcher);

	// These events are sent when their correspending actions happen.
	Event1< const ResourceEvent& > onResourcePrepared;
	Event1< const ResourceEvent& > onResourceLoaded;
	Event1< const ResourceEvent& > onResourceRemoved;
	Event1< const ResourceEvent& > onResourceReloaded;
	Event1< const ResourceLoader&> onResourceLoaderRegistered;

	// Gets an already loaded resource by its name.
	template <typename T>
	RefPtr<T> getResource(const std::string& name)
	{
		ResourcePtr res = getResource(name);
		return RefCast<T>(res);
	}

	// Creates a new resource and returns the specific resource type.
	template <typename T>
	RefPtr<T> loadResource(const std::string& path)
	{
		ResourcePtr res = loadResource(path);
		return RefCast<T>(res);
	}

protected:

	// Finds the true resource if needed.
	void findResource( ResourceLoadOptions& options );

	// Returns a new resource ready to be processed by a loader.
	ResourcePtr prepareResource( const File& file );

	// Processes the resource with the right resource loader.
	void decodeResource( ResourcePtr res, ResourceLoadOptions& options );

	// Validates if the resource exists and if there is a loader for it.
	bool validateResource( const std::string& path );

	// Watches a resource for changes and auto-reloads it.
	void handleWatchResource(const FileWatchEvent& event);

	// Sends pending resource events.
	void sendPendingEvents();

	// References the resource loaders.
	void referenceLoaders();

	// Maps a name to a resource.
	ResourceMap resources;

	// Maps extensions to resource loaders.
	ResourceLoaderMap resourceLoaders;

	// When tasks finish, they queue an event.
	ConcurrentQueue<ResourceEvent> resourceTaskEvents;

	// Keeps track if asynchronous loading is enabled.
	bool asynchronousLoading;

	Mutex resourceFinishLoadMutex;
	Condition resourceFinishLoad;

	// Manages all background loading tasks.
	TaskPool* taskPool;
	FileWatcher* fileWatcher;

	// Number of resources queued for loading.
	VAPOR_ALIGN_BEGIN(32) Atomic VAPOR_ALIGN_END(32) numResourcesQueuedLoad;
};

//-----------------------------------//

class ResourceTask : public Task
{
public:

	void run();

	Resource* resource;
	ResourceLoadOptions options;
};

// Gets the resource manager instance.
ResourceManager* GetResourceManager();

//-----------------------------------//

} // end namespace