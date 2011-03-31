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

#include "Core/Log.h"
#include "Core/Memory.h"
#include "Core/Concurrency.h"
#include "Core/Stream.h"
#include "Core/Archive.h"
#include "Core/FileSystem.h"
#include "Core/Utilities.h"

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
	: taskPool(nullptr)
	, numResourcesQueuedLoad(0)
	, asynchronousLoading(true)
{
	resourcesInstance = this;
	
	MutexInit(&resourceFinishLoadMutex);
	ConditionInit(&resourceFinishLoad);
}

//-----------------------------------//
 
void ResourceManager::setFileWatcher(FileWatcher* watcher)
{
	if( !watcher ) return;
	
	fileWatcher = watcher;
	fileWatcher->onFileWatchEvent.Connect(this, &ResourceManager::handleWatchResource);
}

//-----------------------------------//

ResourceManager::~ResourceManager()
{
	loadQueuedResources();
	assert( resourceTaskEvents.empty() );

	// Delete resource loaders.
	ResourceLoaderMap::const_iterator it;
	for( it = resourceLoaders.begin(); it != resourceLoaders.end(); it++ )
	{
		ResourceLoader* loader = it->second;

		if( !loader )
			continue;

		std::vector<std::string>& extensions = loader->extensions;

		if( extensions.size() == 1 )
			delete loader;
		else
			extensions.erase( std::find(extensions.begin(), extensions.end(), it->first) );
	}

	if( fileWatcher )
		fileWatcher->onFileWatchEvent.Disconnect(this, &ResourceManager::handleWatchResource);

	// Check that all resources will be deleted.
	//foreach( const ResourceMapPair& p, resources )
	//{
	//	assert( p.second->getReferenceCount() == 1 );
	//}
}

//-----------------------------------//

ResourcePtr ResourceManager::getResource(const std::string& path)
{
	Path name = PathGetFile(path);

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

	Stream* stream = StreamCreateFromPhysfs( AllocatorGetDefault(), options.name, StreamMode::Read);
	resource = prepareResource(stream);
	
	if( !resource )
		return nullptr;

	StreamDestroy(stream, AllocatorGetDefault());

	decodeResource(resource, options);

	// Register the decoded resource in the map.

	Path base = PathGetFile(options.name);
	resources[base] = resource;

	return resource;
}

//-----------------------------------//

void ResourceManager::findResource(ResourceLoadOptions& options)
{
	const Path& fileExt = PathGetFileExtension(options.name);
	Path& path = options.name;

	if( !fileExt.empty() )
		return;

	ResourceLoaderMap::const_iterator it;
	for(it = resourceLoaders.begin(); it != resourceLoaders.end(); it++)
	{
		const std::string& ext = it->first;
		ResourceLoader* loader = it->second;

		if( loader->getResourceGroup() != options.group )
			continue;

		Path newPath = StringFormat("%s.%s", path.c_str(), ext.c_str());

		if( FileExists(newPath) )
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
	
	if( !FileExists(path) )
	{
		LogWarn( "Resource '%s' was not found", path.c_str() );
		return false;
	}
	
	const Path& extension = PathGetFileExtension(path);
	
	if( extension.empty() )
	{
		LogWarn( "Resource '%s' has an invalid extension", path.c_str() );
		return false;
	}

	if( !findLoader(extension) )
	{
		LogWarn("No resource loader found for resource '%s'", path.c_str());
		return false;
	}

	return true;
}

//-----------------------------------//

ResourcePtr ResourceManager::prepareResource(Stream* stream)
{
	const Path& path = stream->path;

	// Get the available resource loader and prepare the resource.
	ResourceLoader* loader = findLoader( PathGetFileExtension(path) );

	if( !loader )
	{
		LogWarn("No resource loader found for resource '%s'", path.c_str());
		return nullptr;
	}

	ResourcePtr res( loader->prepare(*stream) );
	res->setStatus( ResourceStatus::Loading );
	res->setPath( path );

	// Send callback notifications.
	ResourceEvent event;
	event.resource = res;
		
	onResourcePrepared(event);

	return res;
}

//-----------------------------------//

void ResourceManager::decodeResource( ResourcePtr resource, ResourceLoadOptions& options )
{
	Task* task = TaskCreate( AllocatorGetDefault() );
	
	ResourceLoadOptions* taskOptions = Allocate<ResourceLoadOptions>( AllocatorGetDefault() );
	*taskOptions = options;
	taskOptions->resource = resource.get();

	task->Callback.Bind(ResourceTaskRun);
	task->Userdata = taskOptions;

	AtomicIncrement(&numResourcesQueuedLoad);;

	if( taskPool && options.asynchronousLoading )
	{
		TaskPoolAdd(taskPool, task);
		return;
	}

	TaskRun(task);
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

void ResourceManager::update( float )
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
	for( it = resources.begin(); it != resources.end(); it++ )
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

	LogInfo("Unloaded resource: %s", path.c_str());
}

//-----------------------------------//

void ResourceManager::registerLoader(ResourceLoader* const loader)
{
	LogInfo( "Registering resource loader '%s'", loader->getName().c_str() );

	const std::vector<std::string>& extensions = loader->getExtensions();
	
	for( uint i = 0; i < extensions.size(); i++ )
	{
		const std::string& extension = extensions[i];

		if(resourceLoaders.find(extension) != resourceLoaders.end())
		{
			LogDebug("Extension '%s' already has a resource loader", extension.c_str());
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

		LogDebug( "Resource was renamed - handle this" );
		return;
	}

	// Register the decoded resource in the map.
	LogInfo("Reloading resource '%s'", file.c_str());

	ResourceLoadOptions options;
	options.sendLoadEvent = false;

	decodeResource( res, options );

	ResourceEvent event;
	event.resource = res;
	
	onResourceReloaded(event);
}

//-----------------------------------//

} // end namespace