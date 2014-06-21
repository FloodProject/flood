/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Resources/API.h"
#include "Resources/Resource.h"

NAMESPACE_RESOURCES_BEGIN

//-----------------------------------//

API_RESOURCE REFLECT_DECLARE_CLASS(ResourceMetadata)

struct API_RESOURCE ResourceMetadata
{
	// Hash of the resource.
	uint32 hash;

	// Path to the resource.
	Path path;

	// Path to preview of the resource.
	Path preview;

	// Group of the resource.
	ResourceGroup group;
};

typedef Vector<ResourceMetadata> ResourcesCache;
typedef HashMap<uint32, ResourceMetadata> ResourcesCacheMap;
typedef std::pair<uint32, ResourceMetadata> ResourcesCacheMapPair;

//-----------------------------------//

class ResourceIndexer;

/**
 * Resource database is used to keep an indexed list of all the resources.
 */

API_RESOURCE REFLECT_DECLARE_CLASS(ResourceDatabase)

class API_RESOURCE ResourceDatabase : public Object
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
	void fixUp() OVERRIDE;

	// Event when a resource is added to the database.
	Event1<const ResourceMetadata&> onResourceAdded;

	// Caches all the resources metadata.
	ResourcesCache resources;
	ResourcesCacheMap resourcesCache;
	ResourceIndexer* indexer;
};

//-----------------------------------//

NAMESPACE_RESOURCES_END