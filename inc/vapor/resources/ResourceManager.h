/************************************************************************
*
* vaporEngine by triton © (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"
#include "vapor/vfs/File.h"

#include "vapor/resources/Resource.h"
#include "vapor/resources/ResourceLoader.h"

namespace vapor {
	namespace resources {

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

class ResourceManager 
{
public:

	ResourceManager();
	virtual ~ResourceManager();

	// Creates a new resource 
	void addResource(File& path);
	
	// Gets an existing resource by its URI (or null if it does not exist).
	Resource* getResource(File& path);

	// Sets a memory budget limit for a given resource group.
	//void setMemoryBudget(ResourceGroup::Enum group, uint memoryBudget);
	
	// Gets the memory usage for a given resource group.
	//uint getMemoryUsage(ResourceGroup::Enum group);

	// Registers a resource handler.
	void registerResourceLoader(ResourceLoader* codec);
	
	// Gets a list of all the registered resource handlers.
	//list<ResourceLoader*> getResourceLoader(string extension);

private:

	// maps a name to a resource
	map<string, Resource*> resources;

	// maps extensions to codec handlers
	// TODO: resource handler list
	map<string, ResourceLoader*> handlers;
};

} } // end namespaces
