/************************************************************************
*
* Flood Project � (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
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
    for( size_t i = 0; i < resources.Size(); i++ )
    {
        const ResourceMetadata& metadata = resources[i];
        resourcesCache[metadata.hash] = metadata;
    }
}

//-----------------------------------//

void ResourceDatabase::addMetadata(const ResourceMetadata& metadata)
{
    if(resourcesCache.Find(metadata.hash) != resourcesCache.End())
        return;

    resources.Push(metadata);
    resourcesCache[metadata.hash] = metadata;

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