/************************************************************************
*
* vapor3D Resources © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Resources/Resource.h"

NAMESPACE_RESOURCES_BEGIN

//-----------------------------------//

REFLECT_DECLARE_CLASS(ResourceMetadata)

struct ResourceMetadata
{
	// Hash of the resource.
	uint32 hash;

	// Path to the resource.
	Path path;

	// Path to preview of the resource.
	Path preview;

	// Group of the resource.
	ResourceGroup::Enum group;
};

typedef std::vector<ResourceMetadata> ResourcesCache;
typedef std::map<uint32, ResourceMetadata> ResourcesCacheMap;
typedef std::pair<uint32, ResourceMetadata> ResourcesCacheMapPair;

//-----------------------------------//

class ResourceIndexer;

/**
 * Resource database is used to keep an indexed list of all the resources.
 */

REFLECT_DECLARE_CLASS(ResourceDatabase)

class ResourceDatabase : public Object
{
	REFLECT_DECLARE_OBJECT(ResourceDatabase)

public:

	ResourceDatabase();
	~ResourceDatabase();
	
	// Adds a new metadata entry to the database.
	void addMetadata(const ResourceMetadata&);

	// Sets the resource indexer.
	void setIndexer(ResourceIndexer*);

	// Gets called when a resource is indexed.
	void onResourceIndexed(const ResourceMetadata&);

	// Serialization fix-up.
	virtual void fixUp() OVERRIDE;

	// Caches all the resources metadata.
	ResourcesCache resources;
	ResourcesCacheMap resourcesCache;
	ResourceIndexer* indexer;
};

//-----------------------------------//

NAMESPACE_RESOURCES_END