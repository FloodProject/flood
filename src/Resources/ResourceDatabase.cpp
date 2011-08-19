/************************************************************************
*
* vapor3D Project © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Resources/API.h"
#include "Resources/ResourceDatabase.h"
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

REFLECT_CLASS(ResourceMetadata)
	FIELD_PRIMITIVE(0, uint32, hash)
	FIELD_PRIMITIVE(1, string, path)
REFLECT_CLASS_END()

REFLECT_CHILD_CLASS(ResourceDatabase, Object)
	FIELD_VECTOR(0, ResourceMetadata, resources)
REFLECT_CLASS_END()

//-----------------------------------//

ResourceDatabase::ResourceDatabase()
{
}

//-----------------------------------//

ResourceDatabase::ResourceDatabase(TaskPool* taskPool)
{
	Task* index = TaskCreate( AllocatorGetThis() );
	index->Callback.Bind(this, &ResourceDatabase::indexResources);
	
	TaskPoolAdd(taskPool, index);
}

//-----------------------------------//

ResourceDatabase::~ResourceDatabase()
{
}

//-----------------------------------//

void ResourceDatabase::fixUp()
{
	for( size_t i = 0; i < resources.size(); i++ )
	{
		const ResourceMetadata& metadata = resources[i];
		resourcesCache[metadata.hash] = metadata;
	}
}

//-----------------------------------//

void ResourceDatabase::scanResources(Archive* archive)
{
	this->archive = archive;

	std::vector<String> found;	
	ArchiveEnumerateFiles(archive, found);
	
	for( size_t i = 0; i < found.size(); i++ )
	{
		resourcesToIndex.push(found[i]);
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

void ResourceDatabase::indexResources(Task* task)
{
	while(true)
	{
		String path;
		resourcesToIndex.wait_and_pop(path);

		ResourceGroup::Enum group;
		
		if( !GetResourceGroupFromPath(path, group) )
		{
			LogWarn("Resource loader for '%s' was not found", path.c_str());
			continue;
		}

		LogDebug("Indexing file '%s'", path.c_str());

		Stream* stream = ArchiveOpenFile(archive, path, AllocatorGetThis());
		
		if( !stream )
		{
			LogWarn("Could not read file '%s'", path.c_str());
			continue;
		}

		std::vector<byte> data;
		StreamRead(stream, data);
		StreamDestroy(stream);

		uint32 hash = HashMurmur2(0xBEEF, &data[0], data.size());
		
		if( resourcesCache.find(hash) != resourcesCache.end() )
			continue;

		ResourceMetadata metadata;
		metadata.hash = hash;
		metadata.path = path;
		metadata.group = group;

		resources.push_back(metadata);
		resourcesCache[hash] = metadata;

		onResourceIndexed(metadata);
	}
}

//-----------------------------------//

NAMESPACE_RESOURCES_END