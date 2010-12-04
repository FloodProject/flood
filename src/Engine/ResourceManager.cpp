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
#include "vapor/vfs/FileSystem.h"
#include "vapor/TaskManager.h"
#include "vapor/Utilities.h"

#include "ResourceTask.h"

namespace vapor {

//-----------------------------------//

ResourceManager::ResourceManager( FileWatcher* fileWatcher, TaskManager* tasks )
	: taskManager(tasks)
	, numResourcesQueuedLoad(0)
	, threadedLoading(true)
{
	assert( taskManager != nullptr );

	if( fileWatcher )
	{
		fileWatcher->onFileWatchEvent.Connect(this, &ResourceManager::handleWatchResource);
	}
}

//-----------------------------------//

ResourceManager::~ResourceManager()
{
	loadQueuedResources();
	assert( resourceTaskEvents.empty() );

	// Delete resource loaders.
	ResourceLoaderMap::const_iterator it;
	for( it = resourceLoaders.cbegin(); it != resourceLoaders.cend(); it++ )
	{
		ResourceLoader* loader = it->second;

		if( !loader )
			continue;

		if( loader->getExtensions().size() == 1 )
			delete loader;
		else
			loader->getExtensions().remove( it->first );
	}

	// Check that all resources will be deleted.
	//foreach( const ResourceMapPair& p, resources )
	//{
	//	assert( p.second->getReferenceCount() == 1 );
	//}
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
	// Check if the resource is already loaded.
	ResourcePtr resource = getResource(path);
	
	if( resource )
		return resource;

	if( !validateResource(path) )
		return nullptr;

	File file(path);
	resource = prepareResource(file);
	
	if( !resource )
		return nullptr;

	decodeResource(resource, async);

	// Register the decoded resource in the map.
	resources[file.getName()] = resource;

	// Send callback notifications.
	ResourceEvent event;
	event.resource = resource;
		
	onResourcePrepared( event );

	return resource;
}

//-----------------------------------//

bool ResourceManager::validateResource( const std::string& path )
{
	if( path.empty() )
		return false;
	
	if( !File::exists(path) )
	{
		Log::warn( "Resource '%s' was not found", path.c_str() );
		return false;
	}

	const std::string& extension = String::getExtensionFromPath(path);
	
	if( extension.empty() )
	{
		Log::warn( "Resource '%s' has an invalid extension", path.c_str() );
		return false;
	}

	if( !findLoader(extension) )
	{
		Log::warn("No resource loader found for resource '%s'", path.c_str());
		return false;
	}

	return true;
}

//-----------------------------------//

ResourcePtr ResourceManager::prepareResource(const File& file)
{
	// Get the available resource loader and prepare the resource.
	ResourceLoader* loader = findLoader(file.getExtension());

	if( !loader )
	{
		Log::warn("No resource loader found for resource '%s'", file.getPath().c_str());
		return nullptr;
	}

	PhysfsStream stream(const_cast<File&>(file));

	ResourcePtr res( loader->prepare(stream) );
	res->setStatus( ResourceStatus::Loading );
	res->setPath( file.getPath() );

	return res;
}

//-----------------------------------//

ResourceLoader* ResourceManager::findLoader(const std::string& ext)
{
	ResourceLoader* loader = resourceLoaders[ext];
	return loader;
}

//-----------------------------------//

void ResourceManager::decodeResource( ResourcePtr resource, bool useThreads, bool notify )
{
	RefPtr<ResourceTask> task = new ResourceTask();

	task->res = resource.get();
	task->rm = this;
	task->notify = notify;

	numResourcesQueuedLoad.inc();

	if( threadedLoading && useThreads )
	{
		taskManager->addTask( task );
		return;
	}
	
	task->run();
}

//-----------------------------------//

void ResourceManager::loadQueuedResources()
{
	THREAD( boost::unique_lock<boost::mutex> lock(resourceFinishLoadMutex); )

	while( numResourcesQueuedLoad.get() > 0 )
	{
		#pragma TODO("Use timed_wait and notify the observers of progress")

		THREAD( resourceFinishLoad.wait(lock); )
		System::sleep( 0.01f );
	}

	assert( numResourcesQueuedLoad.get() == 0 );
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
	sendPendingEvents();
	removeUnusedResources();
}

//-----------------------------------//

void ResourceManager::sendPendingEvents()
{
	// Send resource events.
	ResourceEvent event;

	while( resourceTaskEvents.try_pop(event) )
	{
		onResourceLoaded( event );	
	}
}

//-----------------------------------//

void ResourceManager::removeUnusedResources()
{
	std::vector<std::string> resourcesToRemove;

	// Search for unused resources.
	ResourceMap::const_iterator it;
	for( it = resources.cbegin(); it != resources.cend(); it++ )
	{
		const ResourcePtr& resource = it->second;

		if( resource->getReferenceCount() == 1 )
			resourcesToRemove.push_back(it->first);
	}

	for( uint i = 0; i < resourcesToRemove.size(); i++ )
	{
		const std::string& resource = resourcesToRemove[i];
		removeResource(resource);
	}
}

//-----------------------------------//

void ResourceManager::removeResource(const ResourcePtr& resource)
{
	const std::string& path = resource->getPath();
	removeResource(path);
}

//-----------------------------------//

void ResourceManager::removeResource(const std::string& path)
{
	ResourceMap::iterator it = resources.find(path);
	
	if( it == resources.end() )
		return;

	// Send callback notifications.
	ResourceEvent event;
	event.resource = (*it).second;
		
	onResourceRemoved( event );

	resources.erase(it);

	Log::info("Unloaded resource: %s", path.c_str());
}

//-----------------------------------//

void ResourceManager::registerLoader(ResourceLoader* const loader)
{
	#pragma TODO("Check if the resource loader is already registered")

	// Associate the extensions in the loaders map.
	ExtensionList::const_iterator it;
	for( it = loader->getExtensions().cbegin(); it != loader->getExtensions().cend(); it++ )
	{
		const std::string& extension = *it;
		resourceLoaders[extension] = loader;
	}

	// Send callback notifications.
	onResourceLoaderRegistered( *loader );

	Log::info( "Registered resource loader '%s'", loader->getName().c_str() );
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

		Log::debug( "Resource was renamed - handle this" );
		return;
	}

	// Register the decoded resource in the map.
	Log::info("Reloading resource '%s'", file.c_str());

	decodeResource( res, true, false );

	ResourceEvent re;
	re.resource = res;
	onResourceReloaded( re );
}

//-----------------------------------//

} // end namespace