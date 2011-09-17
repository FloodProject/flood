/************************************************************************
*
* vapor3D Resources © (2008-2010)
*
*	<http://www.vapor3d.org>
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

class ResourceIndexer
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