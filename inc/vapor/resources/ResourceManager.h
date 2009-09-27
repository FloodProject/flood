/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"

#include "vapor/resources/Resource.h"
#include "vapor/resources/ResourceLoader.h"

namespace vapor {
	namespace resources {

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

class VAPOR_API ResourceManager : private boost::noncopyable
{
public:

	ResourceManager();
	virtual ~ResourceManager();

	// Gets an instance of the resource manager.
	static ResourceManager* getInstance();
	//static void setInstance( ResourceManager* rm );

	// Creates a new resource and adds it to the manager.
	ResourcePtr loadResource(const std::string& path);

	// Creates a new resource and returns a more derived pointer.
	template <typename T>
	tr1::shared_ptr<T> loadResource(const std::string& path)
	{
		ResourcePtr res = loadResource( path );
		return tr1::static_pointer_cast< T >( res );
	}

	// Removes a resource from the manager.
	void removeResource(ResourcePtr res);

	// Gets an existing resource by its URI (or null if it does not exist).
	ResourcePtr getResource(const std::string& path);

	// Gets a specific derived pointer to a resource.
	template <typename T>
	tr1::shared_ptr<T> getResource(const std::string& path)
	{
		ResourcePtr res = getResource( path );
		return tr1::static_pointer_cast< T >( res );
	}

	// Sets a memory budget limit for a given resource group.
	//void setMemoryBudget(ResourceGroup::Enum group, uint memoryBudget);
	
	// Gets the memory usage for a given resource group.
	//uint getMemoryUsage(ResourceGroup::Enum group);

	// Registers a resource handler.
	void registerLoader(ResourceLoader* loader);

	// Watches a resource for changes and auto-reloads it.
	void watchResource(ResourcePtr res);
	
	// Gets a list of all the registered resource handlers.
	//std::list<ResourceLoader*> getResourceLoader(string extension);

protected:

	static ResourceManager* rm;

	// maps a name to a resource
	std::map< std::string, ResourcePtr > resources;

	// maps extensions to resource loaders
	std::map< std::string, ResourceLoader* > resourceLoaders;

	// maps each resource group to a specific budget
	std::map< ResourceGroup::Enum, uint > resourceBudgets;
};

//-----------------------------------//



//-----------------------------------//

} } // end namespaces