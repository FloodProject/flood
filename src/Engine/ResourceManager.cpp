/************************************************************************
*
* vaporEngine by triton © (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/resources/ResourceManager.h"

#include "vapor/log/Log.h"

using namespace vapor::log;

namespace vapor {
	namespace resources {

ResourceManager::ResourceManager()
{


}

ResourceManager::~ResourceManager()
{


}

void ResourceManager::addResource(const char* name, Resource* resource)
{
	info("resources", "Registering resource '%s'", name);
	resourceMap[name] = resource;
}

Resource* ResourceManager::getResource(std::string &name)
{
	return resourceMap[name];
}

} } // end namespaces
