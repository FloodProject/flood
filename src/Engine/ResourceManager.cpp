/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/resources/ResourceManager.h"
#include "vapor/TaskManager.h"
#include "vapor/vfs/File.h"
#include "vapor/Utilities.h"

using namespace vapor::vfs;

namespace vapor { namespace resources {

//-----------------------------------//

ResourceManager::ResourceManager()
	: taskManager( nullptr )
{

}

//-----------------------------------//

ResourceManager::~ResourceManager()
{
	// TODO: Maybe this should force all resources to be deleted.

	// Delete resource loaders.
	typedef std::pair< std::string, ResourceLoader* > pair_t;

	// TODO: comment the extension deleting logic.
	foreach( const pair_t& entry, resourceLoaders )
	{
		if( entry.second->getExtensions().size() == 1 )
			delete entry.second;
		else
			entry.second->getExtensions().remove( entry.first );
	}
}

//-----------------------------------//

ResourcePtr ResourceManager::loadResource(const std::string& path)
{
	// Check if the resource is already loaded.
	ResourcePtr res = getResource(path);
	if( res ) return res;

	res = decodeResource(path);
	if( !res ) return res;

	// Register the decoded resource in the map.
	info("resources", "Loaded resource '%s'", path.c_str());

	// Send callback notifications.
	if( !onResourceAdded.empty() )
	{
		ResourceEvent event;
		event.name = path;
		event.resource = res;
		onResourceAdded( event );
	}

	resources[path] = res;
	return res;
}

//-----------------------------------//

class ResourceTask : public Task
{
public:

	void run()
	{
		if( loader->decode( res->getURI(), res ) )
			res->setStatus( ResourceStatus::Loaded );
		else
			res->setStatus( ResourceStatus::Error );

		// Warn that the loader could not decode our resource.
		if( res->getStatus() == ResourceStatus::Error )
		{
			warn("resources", "Resource loader '%s' could not decode resource '%s'",
				loader->getName().c_str(), res->getURI().c_str());
		}
	}

	ResourceLoader* loader;
	Resource* res;
};

//-----------------------------------//

ResourcePtr ResourceManager::decodeResource( const std::string& path )
{
	File file( path );

	if( !file.exists() )
	{
		warn( "resources", "Requested resource '%s' not found", path.c_str() );
		return ResourcePtr();
	}
	
	std::string ext = file.getExtension();
	
	if( ext.empty() )
	{
		warn( "resources", "Requested resource '%s' has an invalid path", path.c_str() );
		return ResourcePtr();
	}

	// Check if we have a resource loader for this extension.
	if( resourceLoaders.find(ext) == resourceLoaders.end() )
	{
		warn( "resources", "No resource loader found for resource '%s'", path.c_str() );
		return ResourcePtr();
	}

	// Get the available resource loader and prepare the resource.
	ResourceLoader* const ldr = resourceLoaders[ext];

	ResourcePtr res( ldr->prepare(path) );
	res->setStatus( ResourceStatus::Loading );
	res->setURI( path );

	ResourceTask* task = new ResourceTask();
	task->res = res.get();
	task->loader = ldr;

	if( taskManager )
		taskManager->addTask( TaskPtr(task) );
	else
		task->run();

	return res;
}

//-----------------------------------//

ResourcePtr ResourceManager::getResource(const std::string& path)
{
	// Check if we have this resource in the map.
	if(resources.find(path) == resources.end()) 
	{
		return ResourcePtr();
	}

	return resources[path];
}

//-----------------------------------//

void ResourceManager::removeResource(const ResourcePtr& res)
{
	ResourceMap::iterator it = resources.begin();
	
	// Check if the resource is in the map.
	while(it != resources.end()) 
	{
		if(it->second == res) 
		{
			// Send callback notifications.
			if( !onResourceRemoved.empty() )
			{
				ResourceEvent event;
				event.resource = res;
				onResourceRemoved( event );
			}

			resources.erase(it);
			return;
		}

		it++;
	}
}

//-----------------------------------//

void ResourceManager::registerLoader(ResourceLoader* const loader)
{
	// TODO: check if the loader is already registered?

	// Associate the extensions in the loaders map.
	foreach( const std::string& ext, loader->getExtensions() )
	{
		resourceLoaders[ext] = loader;
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
	// Check if the filename maps to a known resource.
	const std::string& file = evt.filename;

	if( resources.find(file) == resources.end() )
		return; // Resource is not known.

	const ResourcePtr& res = resources[file];

	// Reload the resource if it was modified.
	if( evt.action != Actions::Modified )
	{
		// TODO: handle renames
		debug( "Resource was renamed - handle this" );
		return;
	}

	// Register the decoded resource in the map.
	info("resources", "Reloading resource '%s'", file.c_str());

	const ResourcePtr& newResource = decodeResource( file );
	if( !newResource ) return;

	// Replace the old resource with the new one.
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

void ResourceManager::setTaskManager( TaskManager* const tm )
{
	if( !tm ) return;
	taskManager = tm;

	//taskManager->onTaskEvent
}

//-----------------------------------//

} } // end namespaces