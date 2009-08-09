/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/resources/ResourceManager.h"

#include "vapor/vfs/File.h"

using namespace vapor::vfs;

namespace vapor {
	namespace resources {

//-----------------------------------//

ResourceManager::ResourceManager()
{

}

//-----------------------------------//

ResourceManager::~ResourceManager()
{
	// TODO: should all resources be deleted here? hmm...

	// delete resource loaders
	std::map< std::string, ResourceLoader* >::iterator it;
	for(it = resourceLoaders.begin(); it != resourceLoaders.end(); it++)
		delete it->second;
}

//-----------------------------------//

shared_ptr<Resource> ResourceManager::createResource(const std::string path)
{
	// check if the resource is already loaded
	shared_ptr<Resource> res = getResource(path);
	if(res != nullptr) return res;

	// test if the file exists
	if( !File::exists( path ) ) 
	{
		warn("resources", "Requested resource '%s' not found", path.c_str());
		return shared_ptr<Resource>();
	}
	
	// check if it has a file extension
	uint ch = path.find_last_of(".");
	
	if(ch == std::string::npos) 
	{
		warn("resources", "Requested resource '%s' has an invalid path", 
			path.c_str());
		return shared_ptr<Resource>();
	}

	// get the file extension
	std::string ext = path.substr(++ch);

	// check if we have a resource loader for this extension
	if(resourceLoaders.find(ext) == resourceLoaders.end())
	{
		warn("resources", "No resource loader found for resource '%s'",
			path.c_str());
		return shared_ptr<Resource>();
	}

	// get the available resource loader to decode the file
	ResourceLoader* ldr = resourceLoaders[ext];
	
	File f(path);
	res = shared_ptr<Resource>( ldr->decode( f ) );
	
	// warn that the loader could not decode our resource
	if(res == nullptr)
	{
		warn("resources", "Resource loader '%s' could not decode resource '%s'", 
			ldr->getName(), path.c_str());
		return shared_ptr<Resource>();
	}

	// register the decoded resource in the map
	info("resources", "Registering resource '%s'", path.c_str());
	resources[path] = res;

	return res;
}

//-----------------------------------//

shared_ptr<Resource> ResourceManager::getResource(const std::string path)
{
	// check if we have this resource in the map
	if(resources.find(path) == resources.end()) 
	{
		return shared_ptr<Resource>();
	}

	return resources[path];
}

//-----------------------------------//

void ResourceManager::removeResource(shared_ptr<Resource> res)
{
	std::map< std::string, shared_ptr<Resource> >::iterator it;
	it = resources.begin();
	
	// check if the resource is in the map
	while(it != resources.end()) 
	{
		if(it->second == res) 
		{
			resources.erase(it);
			return;
		}

		it++;
	}
}

//-----------------------------------//

void ResourceManager::registerLoader(ResourceLoader* loader)
{
	// TODO: check if the loader is already registered?

	// retrieve the extensions the loader recognizes
	std::list< std::string >& extensions = loader->getExtensions();
	std::list< std::string >::iterator it = extensions.begin();
	
	// associate the extensions in the loaders map
	while(it != extensions.end())
	{
		resourceLoaders[*it] = loader;
		it++;
	}

	info( "resources", "Registering resource loader '%s'", 
		loader->getName().c_str(), 
		ResourceGroup::getString( loader->getResourceGroup() ).c_str() );
}

//-----------------------------------//

} } // end namespaces
