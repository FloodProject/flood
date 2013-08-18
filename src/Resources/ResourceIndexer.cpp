/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Resources/API.h"

#ifdef ENABLE_RESOURCE_INDEXER

#include "Resources/ResourceIndexer.h"
#include "Resources/ResourceManager.h"
#include "Resources/ResourceLoader.h"
#include "Core/Archive.h"
#include "Core/Utilities.h"
#include "Core/Concurrency.h"
#include "Core/Stream.h"
#include "Core/Log.h"
#include "Core/Math/Hash.h"

NAMESPACE_RESOURCES_BEGIN

//-----------------------------------//

ResourceIndexer::ResourceIndexer()
{
}

//-----------------------------------//

ResourceIndexer::~ResourceIndexer()
{
}

//-----------------------------------//

void ResourceIndexer::update()
{
	// Send pending events.
	ResourceMetadata metadata;
	while( resourcesIndexed.try_pop_front(metadata) )
	{
		onResourceIndexed(metadata);
	}
}

//-----------------------------------//

static Task* CreateIndexTask(ResourceIndexer* index, const Path& resPath)
{
	Path* path = Allocate(AllocatorGetObject(index), Path);
	path->assign(resPath);

	Task* task = Allocate(AllocatorGetObject(index), Task);
	task->callback.Bind(index, &ResourceIndexer::indexResources);
	task->userdata = path;

	TaskPool* taskPool = GetResourceManager()->getTaskPool();
	taskPool->add(task, 0);

	return task;
}

//-----------------------------------//

void ResourceIndexer::addArchive(Archive* archive)
{
	std::vector<Path> res;
	archive->enumerateFiles(res);
	
	for(auto& i : res)
	{
		Path fullPath = archive->combinePath(i);
		CreateIndexTask(this, fullPath);
	}
}

//-----------------------------------//

static bool GetResourceGroupFromPath(const Path& path, ResourceGroup& group)
{
	String ext = PathGetFileExtension(path);
	
	ResourceManager* res = GetResourceManager();
	
	ResourceLoader* loader = res->findLoader(ext);
	if( !loader ) return false;

	group = loader->getResourceGroup();

	return true;
}

//-----------------------------------//

void ResourceIndexer::indexResources(Task* task)
{
	Path& tempPath = *((Path*) task->userdata);
	Path path = tempPath;
	DeallocateObject(&tempPath);

	Path basePath = PathGetFile(path);

	ResourceGroup group;
		
	if( !GetResourceGroupFromPath(path, group) )
	{
		//LogDebug("Error indexing resource '%s': no loader was found", basePath.c_str());
		return;
	}

	//LogDebug("Indexing file '%s'", basePath.c_str());

	FileStream stream(path, StreamOpenMode::Read);
		
	if( !stream.isValid )
	{
		LogWarn("Error indexing resource '%s': cannot open stream", basePath.c_str());
		return;
	}

	std::vector<byte> data;
	stream.read(data);
	stream.close();

	if( data.empty() )
	{
		LogWarn("Resource '%s' is empty", basePath.c_str());
		return;
	}

	uint32 hash = HashMurmur2(0xBEEF, &data[0], data.size());
		
	ResourceMetadata metadata;
	metadata.hash = hash;
	metadata.path = path;
	metadata.group = group;

	resourcesIndexed.push_back(metadata);
}

//-----------------------------------//

NAMESPACE_RESOURCES_END

#endif