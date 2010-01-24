/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Singleton.h"
#include "vapor/resources/Resource.h"
#include "vapor/resources/ResourceLoader.h"
#include "vapor/vfs/Watcher.h"

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

	// Gets the registered resources.
	const std::map< std::string, ResourcePtr >& getResources();
 
	// Creates a new resource and returns a generic resource.
	ResourcePtr loadResource(const std::string& path);

	// Creates a new resource and returns the specific resource.
	template <typename T>
	RESOURCE_TYPEDECL_FROM_TYPE(T) loadResource(const std::string& path)
	{
		ResourcePtr res = loadResource( path );
		return RESOURCE_SMART_PTR_CAST< T >( res );
	}

	// Removes a resource from the manager.
	void removeResource(ResourcePtr res);

	// Gets an existing resource by its URI (or null if it does not exist).
	ResourcePtr getResource(const std::string& path);

	// Gets a specific resource given it's name (if it exists).
	template <typename T>
	RESOURCE_TYPEDECL_FROM_TYPE(T) getResource(const std::string& path)
	{
		ResourcePtr res = getResource( path );
		return RESOURCE_SMART_PTR_CAST< T >( res );
	}

	// Sets a memory budget limit for a given resource group.
	//void setMemoryBudget(ResourceGroup::Enum group, uint memoryBudget);
	
	// Gets the memory usage for a given resource group.
	//uint getMemoryUsage(ResourceGroup::Enum group);

	// Registers a resource handler.
	void registerLoader(ResourceLoader* loader);

	// Watches a resource for changes and auto-reloads it.
	void handleWatchResource(const vfs::WatchEvent& evt);
	
	// Gets a list of all the registered resource handlers.
	//std::list<ResourceLoader*> getResourceLoader(string extension);

	// These events are sent when their correspending actions happen.
	fd::delegate< void( const ResourceEvent& ) > onResourceAdded;
	fd::delegate< void( const ResourceEvent& ) > onResourceRemoved;
	fd::delegate< void( const ResourceEvent& ) > onResourceReloaded;
	fd::delegate< void( const ResourceLoader& ) > onResourceLoaderRegistered;

protected:

	ResourcePtr decodeResource( const std::string& path );

	// maps a name to a resource
	std::map< std::string, ResourcePtr > resources;

	// maps extensions to resource loaders
	std::map< std::string, ResourceLoader* > resourceLoaders;

	// maps each resource group to a specific budget
	std::map< ResourceGroup::Enum, uint > resourceBudgets;
};

//-----------------------------------//

} } // end namespaces