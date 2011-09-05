/************************************************************************
*
* vapor3D Project © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Resources/API.h"
#include "Resources/ResourceDatabase.h"
#include "Resources/ResourceIndexer.h"
#include "Core/Log.h"

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
{
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

void ResourceDatabase::addMetadata(const ResourceMetadata& metadata)
{
	if(resourcesCache.find(metadata.hash) != resourcesCache.end())
		return;

	resources.push_back(metadata);
	resourcesCache[metadata.hash] = metadata;
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