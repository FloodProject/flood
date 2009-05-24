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

//-----------------------------------//

ResourceManager::ResourceManager()
{

}

//-----------------------------------//

ResourceManager::~ResourceManager()
{

}

//-----------------------------------//

void ResourceManager::addResource(File& path)
{
	//info("resources", "Registering resource '%s'", resource->);
	//resources[path] = resource;
}

//-----------------------------------//

Resource* ResourceManager::getResource(File& path)
{
	//return resources[name];
	return nullptr;
}

//-----------------------------------//

void ResourceManager::registerResourceLoader(ResourceLoader* handler)
{
	list<string>& extensions = handler->getExtensions();

	list<string>::iterator i = extensions.begin();
	
	while(i != extensions.end()) {
		handlers[*i++] = handler;
	}

	info("resources", "Registering resource handler: %s (%s)", 
		handler->getName().c_str(), ResourceGroup::getString(handler->getResourceGroup()));
}

//-----------------------------------//

//ResourceLoader* ResourceManager::getCodec(string extension)
//{
//	return nullptr;
//}

//-----------------------------------//

} } // end namespaces
