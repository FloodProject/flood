/************************************************************************
*
* vapor3D Resources © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Core/ConcurrentQueue.h"
#include "Resources/Resource.h"

NAMESPACE_RESOURCES_BEGIN

//-----------------------------------//

REFLECT_DECLARE_CLASS(ResourceMetadata)

struct ResourceMetadata
{
	// Hash of the resource.
	uint32 hash;

	// Path of the resource.
	String path;

	// Thumbnail of the resource.
	String thumbnail;

	// Group of the resource.
	ResourceGroup::Enum group;
};

typedef std::vector<ResourceMetadata> ResourcesCache;
typedef std::map<uint32, ResourceMetadata> ResourcesCacheMap;
typedef std::pair<uint32, ResourceMetadata> ResourcesCacheMapPair;

//-----------------------------------//

struct Archive;

/**
 * Keeps an indexed list of all the resources known.
 * Indexing is done asynchronously using the task manager.
 */

REFLECT_DECLARE_CLASS(ResourceDatabase)

class ResourceDatabase : public Object
{
	REFLECT_DECLARE_OBJECT(ResourceDatabase)

public:

	ResourceDatabase();
	ResourceDatabase(TaskPool*);
	~ResourceDatabase();

	// Scans for known resources.
	void scanResources(Archive*);

	// Serialization fix-up.
	virtual void fixUp() OVERRIDE;

	// Caches all the resources metadata.
	ResourcesCache resources;
	ResourcesCacheMap resourcesCache;

	Event1<const ResourceMetadata&> onResourceIndexed;

private:

	// Indexes found resources.
	void indexResources(Task*);

	Archive* archive;
	ConcurrentQueue<String> resourcesToIndex;
	ConcurrentQueue<String> resourcesToThumb;
};

//-----------------------------------//

NAMESPACE_RESOURCES_END