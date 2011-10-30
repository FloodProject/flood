/************************************************************************
*
* vapor3D Project © (2008-2010)
*
*	<http://www.vapor3d.org>
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
#include "Math/Hash.h"

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
	Path* path = Allocate(Path, AllocatorGetObject(index));
	path->assign(resPath);

	Task* task = TaskCreate( AllocatorGetObject(index) );
	task->callback.Bind(index, &ResourceIndexer::indexResources);
	task->userdata = path;

	TaskPool* taskPool = GetResourceManager()->getTaskPool();
	TaskPoolAdd(taskPool, task, 0);

	return task;
}

//-----------------------------------//

void ResourceIndexer::addArchive(Archive* archive)
{
	std::vector<String> res;	
	ArchiveEnumerateFiles(archive, res);
	
	for( size_t i = 0; i < res.size(); i++ )
	{
		Path fullPath = ArchiveCombinePath(archive, res[i]);
		CreateIndexTask(this, fullPath);
	}
}

//-----------------------------------//

static bool GetResourceGroupFromPath(const Path& path, ResourceGroup::Enum& group)
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

	ResourceGroup::Enum group;
		
	if( !GetResourceGroupFromPath(path, group) )
	{
		LogDebug("Error indexing resource '%s': no loader was found", basePath.c_str());
		return;
	}

	//LogDebug("Indexing file '%s'", basePath.c_str());

	Stream* stream = StreamCreateFromFile(AllocatorGetThis(), path, StreamMode::Read);
		
	if( !stream )
	{
		LogWarn("Error indexing resource '%s': cannot open stream", basePath.c_str());
		return;
	}

	std::vector<byte> data;
	StreamRead(stream, data);
	StreamDestroy(stream);

	if( data.empty() )
	{
		LogWarn("Error indexing resource '%s': empty stream", basePath.c_str());
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