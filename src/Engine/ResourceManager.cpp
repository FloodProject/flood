/************************************************************************
*
* vaporEngine by triton © (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/resources/ResourceManager.h"

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
	_resourceMap[name] = resource;
}

Resource* ResourceManager::getResource(std::string &name)
{
	return _resourceMap[name];
}

} } // end namespaces
