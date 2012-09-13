/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#ifdef ENABLE_RESOURCE_INDEXER

#include "Core/Concurrency.h"
#include "Resources/Resource.h"
#include "Resources/ResourceDatabase.h"

NAMESPACE_RESOURCES_BEGIN

//-----------------------------------//

struct Archive;
class ResourceDatabase;

/**
 * Monitors the given archives and indexes all new resources.
 */

class API_RESOURCE ResourceIndexer
{
public:

	ResourceIndexer();
	~ResourceIndexer();

	// Adds a new archive to be indexed.
	void addArchive(Archive* archive);

	// Updates the resource indexer.
	void update();

	// Called when a resource is indexed.
	Event1<const ResourceMetadata&> onResourceIndexed;

	// Indexes found resources.
	void indexResources(Task*);

	ConcurrentQueue<ResourceMetadata> resourcesIndexed;
};

//-----------------------------------//

NAMESPACE_RESOURCES_END

#endif