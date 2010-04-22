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
	: taskManager( TaskManager::getInstancePtr() ),
	numResourcesQueuedLoad(0)
{
	assert( taskManager != nullptr );
}

//-----------------------------------//

ResourceManager::~ResourceManager()
{
	assert( resourceTaskEvents.empty() );

	// Delete resource loaders.
	// TODO: comment the extension deleting logic.
	foreach( const ResourceLoaderMapPair& entry, resourceLoaders )
	{
		if( entry.second->getExtensions().size() == 1 )
			delete entry.second;
		else
			entry.second->getExtensions().remove( entry.first );
	}

	// Check that all resources will be deleted.
	//foreach( const ResourceMapPair& p, resources )
		//assert( p.second->getReferenceCount() == 1 );
}

//-----------------------------------//

ResourcePtr ResourceManager::loadResource(const std::string& path, bool async)
{
	// Check if the resource is already loaded.
	ResourcePtr res = getResource(path);
	if( res ) return res;

	res = prepareResource(path);
	if( !res ) return res;

	decodeResource(res, async);

	// Register the decoded resource in the map.
	info("resources", "Loaded resource '%s'", path.c_str());

	// Send callback notifications.
	if( !onResourceAdded.empty() )
	{
		ResourceEvent event;
		event.resource = res;
		onResourceAdded( event );
	}

	resources[path] = res;
	return res;
}

//-----------------------------------//

ResourceLoaderPtr const ResourceManager::getResourceLoader(const std::string& ext)
{
	// Check if we have a resource loader for this extension.
	if( resourceLoaders.find(ext) == resourceLoaders.end() )
		return nullptr;

	ResourceLoaderPtr const loader = resourceLoaders[ext];
	return loader;
}

//-----------------------------------//

class ResourceTask : public Task
{
public:

	void run()
	{
		ResourceManagerPtr const rm = ResourceManager::getInstancePtr();

		const std::string& path = res->getURI();
		
		File file( path );
		const std::string& ext = file.getExtension();
		
		ResourceLoaderPtr const loader = rm->getResourceLoader(ext);

		if( !loader )
		{
			warn( "resources", "No resource loader found for resource '%s'",
				file.getPath().c_str() );
			return;
		}
		
		if( loader->decode(file, res) )
			res->setStatus( ResourceStatus::Loaded );
		else
			res->setStatus( ResourceStatus::Error );

		if( res->getStatus() == ResourceStatus::Error )
		{
			warn("resources", "Resource loader '%s' could not decode resource '%s'",
				loader->getName().c_str(), res->getURI().c_str());
			return;
		}

		ResourceEvent event;
		event.resource = res;
		rm->resourceTaskEvents.push(event);

		rm->numResourcesQueuedLoad--;
		rm->resourceFinishLoad.notify_one();
	}

	Resource* res;
};

//-----------------------------------//

bool ResourceManager::validateResource( const File& file )
{
	const std::string& path = file.getPath();

	if( !file.exists() )
	{
		warn( "resources", "Requested resource '%s' not found", path.c_str() );
		return false;
	}

	std::string ext = file.getExtension();
	
	if( ext.empty() )
	{
		warn( "resources", "Requested resource '%s' has an invalid path", path.c_str() );
		return false;
	}

	return true;
}

//-----------------------------------//

ResourcePtr ResourceManager::prepareResource( const std::string& path )
{
	File file(path);

	if( !validateResource(file) )
		return ResourcePtr();

	// Get the available resource loader and prepare the resource.
	ResourceLoaderPtr const ldr = resourceLoaders[file.getExtension()];

	ResourcePtr res( ldr->prepare(file) );
	res->setStatus( ResourceStatus::Loading );
	res->setURI( path );

	return res;
}

//-----------------------------------//

void ResourceManager::decodeResource( ResourcePtr res, bool async )
{
	ResourceTask* task = new ResourceTask();
	task->res = res.get();

	if( taskManager && async )
	{
		numResourcesQueuedLoad++;
		taskManager->addTask( TaskPtr(task) );
	}
	else
	{
		task->run();
		delete task;
	}
}

//-----------------------------------//

void ResourceManager::waitUntilQueuedResourcesLoad()
{
	boost::unique_lock<boost::mutex> lock(resourceFinishLoadMutex);

	while( numResourcesQueuedLoad > 0 )
	{
		// TODO: use a timed_wait and notify the observers
		// to let them implement things like progress bars
		// on loading screens.
		resourceFinishLoad.wait(lock);
	}
}

//-----------------------------------//

ResourcePtr ResourceManager::getResource(const std::string& path)
{
	// Check if we have this resource in the map.
	if( resources.find(path) == resources.end() ) 
	{
		return ResourcePtr();
	}

	return resources[path];
}

//-----------------------------------//

void ResourceManager::update( double )
{
	ResourceEvent event;
	
	while( resourceTaskEvents.try_pop(event) )
	{
		if( !onResourceLoaded.empty() )
			onResourceLoaded( event );	
	}
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

			it = resources.erase(it);
			return;
		}
		else
		{
			it++;
		}
	}
}

//-----------------------------------//

void ResourceManager::registerLoader(ResourceLoaderPtr const loader)
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

	decodeResource( res );

	if( !onResourceReloaded.empty() )
	{
		ResourceEvent re;
		re.resource = res;
		onResourceReloaded( re );
	}
}

//-----------------------------------//

} } // end namespaces