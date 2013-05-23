/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Resources/API.h"
#include "Resources/ResourceDatabase.h"
#include "Resources/ResourceIndexer.h"
#include "Core/Log.h"
#include "Core/Memory.h"
#include "Core/Containers/Array.h"

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
	: indexer(nullptr)
	, resources(*AllocatorGetHeap())
{
}

//-----------------------------------//

ResourceDatabase::~ResourceDatabase()
{
	for(size_t i = 0; i < array::size(resources); ++i)
		Deallocate(resources[i]);
	array::clear(resources);
}

//-----------------------------------//

void ResourceDatabase::fixUp()
{
	for( size_t i = 0; i < array::size(resources); ++i )
	{
		auto metadata = resources[i];
		resourcesCache[metadata->hash] = *metadata;
	}
}

//-----------------------------------//

void ResourceDatabase::addMetadata(const ResourceMetadata& metadata)
{
	if(resourcesCache.find(metadata.hash) != resourcesCache.end())
		return;

	auto newMetadata = new (AllocatorAllocate(AllocatorGetHeap(), sizeof(ResourceMetadata), alignof(ResourceMetadata))) ResourceMetadata(metadata);

	array::push_back(resources, newMetadata);
	resourcesCache[metadata.hash] = *newMetadata;

	onResourceAdded(metadata);
}

//-----------------------------------//

void ResourceDatabase::onResourceIndexed(const ResourceMetadata& metadata)
{
	addMetadata(metadata);
}

//-----------------------------------//

void ResourceDatabase::setIndexer(ResourceIndexer* indexer)
{
	if( !indexer ) return;

	this->indexer = indexer;
	indexer->onResourceIndexed.Connect(this, &ResourceDatabase::onResourceIndexed);
}

//-----------------------------------//

NAMESPACE_RESOURCES_END