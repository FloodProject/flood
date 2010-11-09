/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/Subsystem.h"
#include "vapor/ConcurrentQueue.h"
#include "vapor/resources/Resource.h"

namespace vapor {

//-----------------------------------//

class File;
class FileWatcher;
class FileWatchEvent;

class TaskManager;
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
};

//-----------------------------------//

typedef std::map< std::string, ResourcePtr > ResourceMap;
typedef std::pair< const std::string, ResourcePtr > ResourceMapPair;

typedef std::map< std::string, ResourceLoader* > ResourceLoaderMap;
typedef std::pair< std::string, ResourceLoader* > ResourceLoaderMapPair;

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

class VAPOR_API ResourceManager : public Subsystem
{
	friend class ResourceTask;

public:

	ResourceManager( FileWatcher* fileWatcher, TaskManager* );
	virtual ~ResourceManager();
 
	// Creates a new resource and returns it.
	ResourcePtr loadResource(const std::string& path, bool async = true);

	// Gets an already loaded resource by its path.
	ResourcePtr getResource(const std::string& path);

	// Finds the loader for the given extension.
	ResourceLoader* findLoader(const std::string& ext);

	// Validates if the resource exists and if there is a loader for it.
	bool validateResource( const std::string& path );

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

	// Watches a resource for changes and auto-reloads it.
	void handleWatchResource(const FileWatchEvent& evt);

	// Gets a registered resource loader for the given extension.
	ResourceLoader* const getResourceLoader(const std::string& ext);

	// Sends resource events to the subscribers.
	void update( double );

	// Gets the registered resources.
	GETTER(Resources, const ResourceMap&, resources)

	// Gets the registered resource loaders.
	GETTER(ResourceLoaders, const ResourceLoaderMap&, resourceLoaders)

	// Gets/sets the threading status.
	ACESSOR(ThreadedLoading, bool, threadedLoading)

protected:

	// Returns a new resource ready to be processed by a loader.
	ResourcePtr prepareResource( const File& file );

	// Processes the resource with the right resource loader.
	void decodeResource( ResourcePtr res, bool async = true, bool notify = true );

	// Sends pending resource events.
	void sendPendingEvents();

	// Maps a name to a resource.
	ResourceMap resources;

	// Maps extensions to resource loaders.
	ResourceLoaderMap resourceLoaders;

	// Manages all background loading tasks.
	TaskManager* taskManager;

	// When tasks finish, they queue an event.
	concurrent_queue<ResourceEvent> resourceTaskEvents;

	// Number of resources queued for loading.
	atomic_int numResourcesQueuedLoad;

	// Global setting to override threaded loading.
	bool threadedLoading;

	THREAD(boost::mutex resourceFinishLoadMutex;)
	THREAD(boost::condition_variable resourceFinishLoad;)

public:

	// These events are sent when their correspending actions happen.
	Event1< const ResourceEvent& > onResourcePrepared;
	Event1< const ResourceEvent& > onResourceLoaded;
	Event1< const ResourceEvent& > onResourceRemoved;
	Event1< const ResourceEvent& > onResourceReloaded;
	Event1< const ResourceLoader&> onResourceLoaderRegistered;

	// Gets a specific resource given it's name (if it exists).
	template <typename T>
	boost::intrusive_ptr<T> getResource(const std::string& path)
	{
		ResourcePtr res = getResource( path );
		return boost::static_pointer_cast<T>( res );
	}

	// Creates a new resource and returns the specific resource type.
	template <typename T>
	boost::intrusive_ptr<T> loadResource(const std::string& path, bool async = true)
	{
		ResourcePtr res = loadResource( path, async );
		return boost::static_pointer_cast<T>( res );
	}
};

//-----------------------------------//

} // end namespace