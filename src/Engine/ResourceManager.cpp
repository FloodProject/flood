/************************************************************************
*
* vaporEngine (2008-2009)
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
	// should all resources be deleted here? hmm...
}

//-----------------------------------//

Resource* ResourceManager::createResource(File& file)
{
	// check if the file is already loaded
	Resource* res = getResource(file);
	if(res != nullptr) return res;

	// test if the file exists
	if(!file.exists()) {
		warn("resources", "Requested resource '%s' not found", 
			file.getPath().c_str());
		return nullptr;
	}
	
	// check if it has a file extension
	uint ch = file.getPath().find_last_of(".");
	if(ch == string::npos) {
		warn("resources", "Requested resource '%s' has an invalid path",
			file.getPath().c_str());
		return nullptr;
	}

	// get the file extension
	string ext = file.getPath().substr(++ch);

	// check if we have a resource loader for this resource
	if(resourceLoaders.find(ext) == resourceLoaders.end()) {
		warn("resources", "No resource loader found for resource '%s'",
			file.getPath().c_str());
		return nullptr;
	}

	// get the available resource loader to decode the file
	ResourceLoader* ldr = resourceLoaders[ext];
	res = ldr->decode(file);
	
	// warn that the loader could not decode our resource
	if(res == nullptr) {
		warn("resources", "Resource loader '%s' could not decode resource '%s'", 
			ldr->getName(), file.getPath().c_str());
		return nullptr;
	}

	// register the decoded resource in the map
	info("resources", "Registering resource '%s'", file.getPath().c_str());
	resources[file.getPath()] = res;

	return res;
}

//-----------------------------------//

Resource* ResourceManager::getResource(File& file)
{
	// check if we have this resource in the map
	if(resources.find(file.getPath()) == resources.end()) {
		return nullptr;
	}

	return resources[file.getPath()];
}

//-----------------------------------//

void ResourceManager::removeResource(Resource *res)
{
	map<string, Resource*>::iterator it = resources.begin();
	
	// check if the resource is in the map
	while(it != resources.end()) {
		if(it->second == res) {
			resources.erase(it);
			return;
		}
		it++;
	}
}

//-----------------------------------//

void ResourceManager::registerResourceLoader(ResourceLoader* loader)
{
	// TODO: check if the loader is already registered?

	// retrieve the extensions the loader recognizes
	list<string>& extensions = loader->getExtensions();
	list<string>::iterator it = extensions.begin();
	
	// associate the extensions in the loaders map
	while(it != extensions.end()) {
		resourceLoaders[*it] = loader;
		it++;
	}

	info("resources", "Registering resource handler '%s' (%s)", 
		loader->getName().c_str(), ResourceGroup::getString(loader->getResourceGroup()).c_str());
}

//-----------------------------------//

} } // end namespaces
