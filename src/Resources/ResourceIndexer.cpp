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
	while( resourcesIndexed.try_pop(metadata) )
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
	TaskPoolAdd(taskPool, task);

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

	ResourceGroup::Enum group;
		
	if( !GetResourceGroupFromPath(path, group) )
	{
		//LogWarn("Resource loader for '%s' was not found", path.c_str());
		return;
	}

	//LogDebug("Indexing file '%s'", path.c_str());

	Stream* stream = StreamCreateFromFile(AllocatorGetThis(), path, StreamMode::Read);
		
	if( !stream )
	{
		LogWarn("Could not read file '%s'", path.c_str());
		return;
	}

	std::vector<byte> data;
	StreamRead(stream, data);
	StreamDestroy(stream);

	uint32 hash = HashMurmur2(0xBEEF, &data[0], data.size());
		
	ResourceMetadata metadata;
	metadata.hash = hash;
	metadata.path = path;
	metadata.group = group;

	resourcesIndexed.push(metadata);
}

//-----------------------------------//

NAMESPACE_RESOURCES_END

#endif