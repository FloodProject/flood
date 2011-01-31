/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Resources/API.h"
#include "Resources/ResourceManager.h"
#include "Resources/ResourceLoader.h"

#include "Core/FileSystem.h"
#include "Core/PhysfsStream.h"

#include "TaskManager.h"
#include "Utilities.h"

namespace vapor {

//-----------------------------------//

static ResourceManager* resourcesInstance;
ResourceManager* GetResourceManager() { return resourcesInstance; }

//-----------------------------------//

ResourceLoadOptions::ResourceLoadOptions()
	: group(ResourceGroup::General)
	, asynchronousLoading(true)
	, sendLoadEvent(true)
{ }

//-----------------------------------//

ResourceManager::ResourceManager()
	: taskManager(nullptr)
	, numResourcesQueuedLoad(0)
	, asynchronousLoading(true)
{
	resourcesInstance = this;
}

//-----------------------------------//
 
void ResourceManager::setFileWatcher(FileWatcher* watcher)
{
	if( !watcher )
		return;

	watcher->onFileWatchEvent.Connect(this, &ResourceManager::handleWatchResource);
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

	//if( fileWatcher )
		//fileWatcher->onFileWatchEvent.Disconnect(this, &ResourceManager::handleWatchResource);

	// Check that all resources will be deleted.
	//foreach( const ResourceMapPair& p, resources )
	//{
	//	assert( p.second->getReferenceCount() == 1 );
	//}
}

//-----------------------------------//

ResourcePtr ResourceManager::getResource(const std::string& path)
{
	Path name = PathUtils::getFile(path);

	if( resources.find(name) == resources.end() )
		return nullptr;

	return resources[name];
}

//-----------------------------------//

ResourcePtr ResourceManager::loadResource(const std::string& name)
{
	ResourceLoadOptions options;
	options.name = name;
	options.asynchronousLoading = asynchronousLoading;
	
	return loadResource(options);
}

//-----------------------------------//

ResourcePtr ResourceManager::loadResource(ResourceLoadOptions options)
{
	findResource(options);

	// Check if the resource is already loaded.
	ResourcePtr resource = getResource(options.name);
	
	if( resource )
		return resource;

	if( !validateResource(options.name) )
		return nullptr;

	File file(options.name);
	resource = prepareResource(file);
	
	if( !resource )
		return nullptr;

	decodeResource(resource, options);

	// Register the decoded resource in the map.
	resources[file.getName()] = resource;

	return resource;
}

//-----------------------------------//

void ResourceManager::findResource(ResourceLoadOptions& options)
{
	const Path& fileExt = PathUtils::getExtension(options.name);
	Path& path = options.name;

	if( !fileExt.empty() )
		return;

	ResourceLoaderMap::const_iterator it;
	for(it = resourceLoaders.cbegin(); it != resourceLoaders.cend(); it++)
	{
		const std::string& ext = it->first;
		ResourceLoader* loader = it->second;

		if( loader->getResourceGroup() != options.group )
			continue;

		Path newPath = String::format("%s.%s", path.c_str(), ext.c_str());

		if( File::exists(newPath) )
		{
			path = newPath;
			break;
		}
	}
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

	const Path& extension = PathUtils::getExtension(path);
	
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

	// Send callback notifications.
	ResourceEvent event;
	event.resource = res;
		
	onResourcePrepared(event);

	return res;
}

//-----------------------------------//

void ResourceManager::decodeResource( ResourcePtr resource, ResourceLoadOptions& options )
{
	RefPtr<ResourceTask> task = new ResourceTask();

	task->resource = resource.get();
	task->options = options;

	numResourcesQueuedLoad.inc();

	if( taskManager && options.asynchronousLoading )
	{
		taskManager->addTask( task );
		return;
	}

	task->run();
	sendPendingEvents();
}

//-----------------------------------//

void ResourceManager::loadQueuedResources()
{
#ifdef VAPOR_THREADING
	MutexLock lock(resourceFinishLoadMutex);

	while( numResourcesQueuedLoad.get() > 0 )
	{
		#pragma TODO("Use timed_wait and notify the observers of progress")

		THREAD( resourceFinishLoad.wait(lock); )
		System::sleep( 0.01f );
	}

	assert( numResourcesQueuedLoad.get() == 0 );
#endif
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
	Log::info( "Registering resource loader '%s'", loader->getName().c_str() );

	const ExtensionList& extensions = loader->getExtensions();
	ExtensionList::const_iterator it;

	for( it = extensions.cbegin(); it != extensions.cend(); it++ )
	{
		const std::string& extension = *it;

		if(resourceLoaders.find(extension) != resourceLoaders.end())
		{
			Log::debug("Extension '%s' already has a resource loader", extension.c_str());
			continue;
		}

		resourceLoaders[extension] = loader;
	}

	// Send callback notifications.
	onResourceLoaderRegistered( *loader );
}

//-----------------------------------//

ResourceLoader* ResourceManager::findLoader(const std::string& ext)
{
	// Check if we have a resource loader for this extension.
	if( resourceLoaders.find(ext) == resourceLoaders.end() )
		return nullptr;

	ResourceLoader* loader = resourceLoaders[ext];
	return loader;
}

//-----------------------------------//

void ResourceManager::setupResourceLoaders()
{
	Class& type = ResourceLoader::getStaticType();
	
	for( uint i = 0; i < type.childs.size(); i++ )
	{
		Class& child = *type.childs[i];
	
		ResourceLoader* loader = (ResourceLoader*) child.createInstance();
		registerLoader( loader );
	}

	// Dummy call so linker doesn't strip the loaders.
	referenceLoaders();
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

	ResourceLoadOptions options;
	options.sendLoadEvent = false;

	decodeResource( res, options );

	ResourceEvent event;
	event.resource = res;
	
	onResourceReloaded(event);
}

//-----------------------------------//

} // end namespace