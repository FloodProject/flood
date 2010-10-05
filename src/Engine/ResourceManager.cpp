/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/resources/ResourceManager.h"
#include "vapor/resources/ResourceLoader.h"

#include "vapor/vfs/File.h"
#include "vapor/vfs/FileWatcher.h"
#include "vapor/vfs/FileSystem.h"

#include "vapor/TaskManager.h"
#include "ResourceTask.h"

namespace vapor {

//-----------------------------------//

ResourceManager::ResourceManager( FileWatcher* fileWatcher, TaskManager* tasks )
	: taskManager(tasks)
	, numResourcesQueuedLoad(0)
{
	assert( taskManager != nullptr );

	if( fileWatcher )
	{
		fileWatcher->onFileWatchEvent +=
			fd::bind(&ResourceManager::handleWatchResource, this);
	}
}

//-----------------------------------//

ResourceManager::~ResourceManager()
{
	waitUntilQueuedResourcesLoad();
	assert( resourceTaskEvents.empty() );

	// Delete resource loaders.
	#pragma TODO("Document the extension deleting logic.")

	foreach( const ResourceLoaderMapPair& entry, resourceLoaders )
	{
		ResourceLoader* loader = entry.second;

		if( !loader )
			continue;

		if( loader->getExtensions().size() == 1 )
			delete loader;
		else
			loader->getExtensions().remove( entry.first );
	}

	// Check that all resources will be deleted.
	//foreach( const ResourceMapPair& p, resources )
		//assert( p.second->getReferenceCount() == 1 );
}

//-----------------------------------//

ResourcePtr ResourceManager::getResource(const std::string& path)
{
	// Check if we have this resource in the map.
	if( resources.find(path) == resources.end() ) 
		return nullptr;

	return resources[path];
}

//-----------------------------------//

ResourcePtr ResourceManager::loadResource(const std::string& path, bool async)
{
	if( path.empty() )
		return nullptr;

	// Check if the resource is already loaded.
	ResourcePtr resource = getResource(path);
	
	if( resource )
		return resource;

	File file(path);
	resource = prepareResource(file);
	
	if( !resource )
		return resource;

	decodeResource(resource, async);

	// Register the decoded resource in the map.
	info("resources", "Loaded resource '%s'", file.getName().c_str());

	// Send callback notifications.
	if( !onResourceAdded.empty() )
	{
		ResourceEvent event;
		event.resource = resource;
		onResourceAdded( event );
	}

	resources[file.getName()] = resource;
	return resource;
}

//-----------------------------------//

bool ResourceManager::validateResource( const File& file )
{
	const std::string& path = file.getPath();

	if( !file.exists() )
	{
		warn( "resources", "Resource '%s' was not found", path.c_str() );
		return false;
	}

	const std::string& extension = file.getExtension();
	
	if( extension.empty() )
	{
		warn( "resources", "Resource '%s' has an invalid extension", path.c_str() );
		return false;
	}

	return true;
}

//-----------------------------------//

ResourcePtr ResourceManager::prepareResource(const File& file)
{
	if( !validateResource(file) )
		return nullptr;

	// Get the available resource loader and prepare the resource.
	ResourceLoader* loader = resourceLoaders[file.getExtension()];

	if( !loader )
	{
		warn("resources", "No resource loader found for resource '%s'", file.getPath().c_str());
		return nullptr;
	}

	ResourcePtr res( loader->prepare(file) );
	res->setStatus( ResourceStatus::Loading );
	res->setPath( file.getPath() );

	return res;
}

//-----------------------------------//

void ResourceManager::decodeResource( ResourcePtr resource, bool async, bool notify )
{
	boost::intrusive_ptr<ResourceTask> task = new ResourceTask();

	task->res = resource.get();
	task->rm = this;
	task->notify = notify;

	numResourcesQueuedLoad++;

	if( async )
		taskManager->addTask( task );
	else
		task->run();
}

//-----------------------------------//

void ResourceManager::waitUntilQueuedResourcesLoad()
{
	THREAD( boost::unique_lock<boost::mutex> lock(resourceFinishLoadMutex); )

	while( numResourcesQueuedLoad > 0 )
	{
		#pragma TODO("Use timed_wait and notify the observers of progress")

		THREAD( resourceFinishLoad.wait(lock); )
		Timer::sleep( 0.01f );
	}

	assert( numResourcesQueuedLoad == 0 );
}

//-----------------------------------//

ResourceLoader* const ResourceManager::getResourceLoader(const std::string& ext)
{
	// Check if we have a resource loader for this extension.
	if( resourceLoaders.find(ext) == resourceLoaders.end() )
		return nullptr;

	ResourceLoader* const loader = resourceLoaders[ext];
	return loader;
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
	ResourceMap::iterator it = resources.find( res->getPath() );
	
	if( it == resources.end() )
		return;

	// Send callback notifications.
	if( !onResourceRemoved.empty() )
	{
		ResourceEvent event;
		event.resource = res;
		onResourceRemoved( event );
	}

	resources.erase(it);
}

//-----------------------------------//

void ResourceManager::registerLoader(ResourceLoader* const loader)
{
	#pragma TODO("Check if the resource loader is already registered")

	// Associate the extensions in the loaders map.
	foreach( const std::string& extension, loader->getExtensions() )
		resourceLoaders[extension] = loader;

	// Send callback notifications.
	if( !onResourceLoaderRegistered.empty() )
		onResourceLoaderRegistered( *loader );

	info( "resources", "Registering resource loader '%s'", loader->getName().c_str() );
}

//-----------------------------------//

void ResourceManager::handleWatchResource(const FileWatchEvent& evt)
{
	// Check if the filename maps to a known resource.
	const std::string& file = evt.filename;

	if( resources.find(file) == resources.end() )
		return; // Resource is not known.

	const ResourcePtr& res = resources[file];

	// Reload the resource if it was modified.
	if( evt.action != Actions::Modified )
	{
		#pragma TODO("Add rename support in live updating")

		debug( "Resource was renamed - handle this" );
		return;
	}

	// Register the decoded resource in the map.
	info("resources", "Reloading resource '%s'", file.c_str());

	decodeResource( res, true, false );

	if( !onResourceReloaded.empty() )
	{
		ResourceEvent re;
		re.resource = res;
		onResourceReloaded( re );
	}
}

//-----------------------------------//

} // end namespace