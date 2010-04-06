/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/Singleton.h"
#include "vapor/resources/Resource.h"
#include "vapor/resources/ResourceLoader.h"
#include "vapor/vfs/Watcher.h"

namespace vapor { class TaskManager; }

namespace vapor { namespace resources {

//-----------------------------------//

/**
 * This event gets sent out whenever an operation that has the corresponding
 * delegate declared in the Resource class is executed. This can be useful 
 * when monitoring resource changes is (for example, in editor applications).
 */

struct ResourceEvent
{
	std::string name;
	ResourcePtr resource;
	ResourcePtr newResource;
};

//-----------------------------------//

typedef std::map< std::string, ResourcePtr > ResourceMap;

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

class VAPOR_API ResourceManager 
	: public Singleton<ResourceManager>, private boost::noncopyable
{
public:

	ResourceManager();
	virtual ~ResourceManager();
 
	// Creates a new resource and returns an handle to the resource.
	ResourcePtr loadResource(const std::string& path);

	// Creates a new resource and returns the specific resource type.
	template <typename T>
	RESOURCE_TYPEDECL_FROM_TYPE(T) loadResource(const std::string& path)
	{
		ResourcePtr res = loadResource( path );
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

	// Gets the registered resources.
	IMPLEMENT_GETTER(Resources, const ResourceMap&, resources)

	// Registers a resource handler.
	void registerLoader(ResourceLoader* const loader);

	// Watches a resource for changes and auto-reloads it.
	void handleWatchResource(const vfs::WatchEvent& evt);

	// Sets the task manager instance that the resource manager will use.
	void setTaskManager( TaskManager* const );

	// These events are sent when their correspending actions happen.
	fd::delegate< void( const ResourceEvent& ) > onResourceAdded;
	fd::delegate< void( const ResourceEvent& ) > onResourceRemoved;
	fd::delegate< void( const ResourceEvent& ) > onResourceReloaded;
	fd::delegate< void( const ResourceLoader& ) > onResourceLoaderRegistered;

protected:

	ResourcePtr decodeResource( const std::string& path );

	// Maps a name to a resource.
	ResourceMap resources;

	// Maps extensions to resource loaders.
	typedef std::map< std::string, ResourceLoader* > ResourceLoaderMap;
	ResourceLoaderMap resourceLoaders;

	// Manages all background loading tasks.
	TaskManager* taskManager;
};

//-----------------------------------//

} } // end namespaces