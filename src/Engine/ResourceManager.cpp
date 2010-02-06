/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/resources/ResourceManager.h"
#include "vapor/vfs/File.h"
#include "vapor/Utilities.h"

using namespace vapor::vfs;

namespace vapor { namespace resources {

//-----------------------------------//

ResourceManager::ResourceManager()
{

}

//-----------------------------------//

ResourceManager::~ResourceManager()
{
	// TODO: should all resources be deleted here? hmm...

	// delete resource loaders
	typedef std::pair< std::string, ResourceLoader* > pair_t;

	// TODO: comment the extension deleting logic,
	// I don't remember anymore how it works...

	foreach( pair_t entry, resourceLoaders )
	{
		if( entry.second->getExtensions().size() == 1 )
			delete entry.second;
		else
			entry.second->getExtensions().remove( entry.first );
	}
}

//-----------------------------------//

const std::map< std::string, ResourcePtr >& ResourceManager::getResources()
{
	return resources;
}

//-----------------------------------//

ResourcePtr ResourceManager::loadResource(const std::string& path)
{
	// check if the resource is already loaded
	ResourcePtr res = getResource(path);
	if(res != nullptr) return res;

	res = decodeResource(path);
	if( !res ) return res;

	// register the decoded resource in the map
	info("resources", "Loaded resource '%s'", path.c_str());

	// Send callback notifications.
	if( !onResourceAdded.empty() )
	{
		ResourceEvent event;
		event.name = path;
		event.resource = res;
		onResourceAdded( event );
	}

	res->setURI( path );
	resources[path] = res;
	return res;
}

//-----------------------------------//

ResourcePtr ResourceManager::decodeResource( const std::string& path )
{
	// test if the file exists
	if( !File::exists( path ) ) 
	{
		warn("resources", "Requested resource '%s' not found", path.c_str());
		return ResourcePtr();
	}
	
	// check if it has a file extension
	uint ch = path.find_last_of(".");
	
	if(ch == std::string::npos) 
	{
		warn("resources", "Requested resource '%s' has an invalid path", 
			path.c_str());
		return ResourcePtr();
	}

	// get the file extension
	std::string ext = path.substr(++ch);

	// check if we have a resource loader for this extension
	if(resourceLoaders.find(ext) == resourceLoaders.end())
	{
		warn("resources", "No resource loader found for resource '%s'",
			path.c_str());
		return ResourcePtr();
	}

	// get the available resource loader to decode the file
	ResourceLoader* ldr = resourceLoaders[ext];
	ResourcePtr res = ResourcePtr( ldr->decode( File(path) ) );
	
	// warn that the loader could not decode our resource
	if(res == nullptr)
	{
		warn("resources", "Resource loader '%s' could not decode resource '%s'", 
			ldr->getName().c_str(), path.c_str());
		return ResourcePtr();
	}

	return res;
}

//-----------------------------------//

ResourcePtr ResourceManager::getResource(const std::string& path)
{
	// check if we have this resource in the map
	if(resources.find(path) == resources.end()) 
	{
		return ResourcePtr();
	}

	return resources[path];
}

//-----------------------------------//

void ResourceManager::removeResource(ResourcePtr res)
{
	std::map< std::string, ResourcePtr >::iterator it;
	it = resources.begin();
	
	// check if the resource is in the map
	while(it != resources.end()) 
	{
		if(it->second == res) 
		{
			// Send callback notifications.
			if( !onResourceRemoved.empty() )
			{
				ResourceEvent event;
				event.resource = res;
				//event.name = ;
				onResourceRemoved( event );
			}

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

	// Send callback notifications.
	if( !onResourceLoaderRegistered.empty() )
	{
		onResourceLoaderRegistered( *loader );
	}

	info( "resources", "Registering resource loader '%s'", 
		loader->getName().c_str(), 
		ResourceGroup::getString( loader->getResourceGroup() ).c_str() );
}

//-----------------------------------//

void ResourceManager::handleWatchResource(const vfs::WatchEvent& evt)
{
	// Check if the filename maps to a known resource
	std::string file = wstrtostr( evt.filename );

	if( resources.find(file) == resources.end() )
		return; // Resource is not known

	ResourcePtr res = resources[file];

	// Reload the resource if it was modified
	if( evt.action != Actions::Modified )
	{
		// TODO: handle renames
		debug( "Resource was renamed - handle this" );
		return;
	}

	// register the decoded resource in the map
	info("resources", "Reloading resource '%s'", file.c_str());

	ResourcePtr newResource = decodeResource( file );
	if( !newResource ) return;

	// Replace the old resource with the new one
	resources[file] = newResource;

	if( !onResourceReloaded.empty() )
	{
		ResourceEvent re;
		re.name = file;
		re.resource = res;
		re.newResource = newResource;
		onResourceReloaded( re );
	}
}

//-----------------------------------//

} } // end namespaces