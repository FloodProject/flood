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
#include "vapor/resources/ResourceLoader.h"
#include "vapor/vfs/Watcher.h"

FWD_DECL_TYPEDEF_PTR(TaskManager)

namespace vapor { namespace resources {

class ResourceTask;

//-----------------------------------//

/**
 * This event gets sent out whenever an operation that has the corresponding
 * delegate declared in the Resource class is executed. This can be useful 
 * when monitoring resource changes is (for example, in editor applications).
 */

struct ResourceEvent
{
	ResourcePtr resource;
};

//-----------------------------------//

typedef std::map< std::string, ResourcePtr > ResourceMap;
typedef std::pair< const std::string, ResourcePtr > ResourceMapPair;

typedef std::map< std::string, ResourceLoaderPtr > ResourceLoaderMap;
typedef std::pair< std::string, ResourceLoaderPtr > ResourceLoaderMapPair;
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

	ResourceManager();
	virtual ~ResourceManager();
 
	// Creates a new resource and returns an handle to the resource.
	ResourcePtr loadResource(const std::string& path, bool async = true);

	// Creates a new resource and returns the specific resource type.
	template <typename T>
	RESOURCE_TYPEDECL_FROM_TYPE(T) loadResource(const std::string& path, bool async = true)
	{
		ResourcePtr res = loadResource( path, async );
		return RESOURCE_SMART_PTR_CAST< T >( res );
	}

	// Gets an existing resource by its URI (or null if it does not exist).
	ResourcePtr getResource(const std::string& path);

	// Gets a specific resource given it's name (if it exists).
	template <typename T>
	RESOURCE_TYPEDECL_FROM_TYPE(T) getResource(const std::string& path)
	{
		ResourcePtr res = getResource( path );
		return RESOURCE_SMART_PTR_CAST< T >( res );
	}

	// Removes a resource from the manager.
	void removeResource(const ResourcePtr& res);

	// Finishes until all queued resources are loaded.
	void waitUntilQueuedResourcesLoad();

	// Registers a resource handler.
	void registerLoader(ResourceLoaderPtr const loader);

	// Watches a resource for changes and auto-reloads it.
	void handleWatchResource(const vfs::WatchEvent& evt);

	// Gets a registered resource loader for the given extension.
	ResourceLoaderPtr const getResourceLoader(const std::string& ext);

	// Sends resource events to the subscribers.
	void update( double );

	// Gets the registered resources.
	IMPLEMENT_GETTER(Resources, const ResourceMap&, resources)

	// Gets the registered resource loaders.
	IMPLEMENT_GETTER(ResourceLoaders, const ResourceLoaderMap&, resourceLoaders)

protected:

	// Validates that the resource exists and there is a loader for it.
	bool validateResource( const vfs::File& file );

	// Returns a new resource ready to be processed by a loader.
	ResourcePtr prepareResource( const std::string& path );

	// Processes the resource with the right resource loader.
	void decodeResource( ResourcePtr res, bool async = true );

	// Maps a name to a resource.
	ResourceMap resources;

	// Maps extensions to resource loaders.
	ResourceLoaderMap resourceLoaders;

	// Manages all background loading tasks.
	TaskManagerPtr taskManager;

	// When tasks finish, they queue a loaded event into the queue.
	concurrent_queue<ResourceEvent> resourceTaskEvents;
	atomic_int numResourcesQueuedLoad;

#ifdef VAPOR_THREADING
	boost::mutex resourceFinishLoadMutex;
	boost::condition_variable resourceFinishLoad;
#endif

public:

	// These events are sent when their correspending actions happen.
	fd::delegate< void( const ResourceEvent& ) > onResourceAdded;
	fd::delegate< void( const ResourceEvent& ) > onResourceLoaded;
	fd::delegate< void( const ResourceEvent& ) > onResourceRemoved;
	fd::delegate< void( const ResourceEvent& ) > onResourceReloaded;
	fd::delegate< void( const ResourceLoader& ) > onResourceLoaderRegistered;
};

TYPEDEF_PTR(ResourceManager)

//-----------------------------------//

} } // end namespaces