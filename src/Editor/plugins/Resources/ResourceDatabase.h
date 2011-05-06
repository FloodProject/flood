/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

namespace vapor { namespace editor {

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
};

typedef std::vector<ResourceMetadata> ResourcesCache;
typedef std::map<uint32, ResourceMetadata> ResourcesCacheMap;
typedef std::pair<uint32, ResourceMetadata> ResourcesCacheMapPair;

//-----------------------------------//

REFLECT_DECLARE_CLASS(ResourceDatabase)

class ResourceDatabase : public Object
{
	REFLECT_DECLARE_OBJECT(ResourceDatabase)

public:

	ResourceDatabase();
	~ResourceDatabase();

	// Loads the thumbnails cache.
	bool loadCache();

	// Saves the thumbnails cache.
	bool saveCache();

	// Scans for known resources.
	void scanFiles();

	// Serialization fix-up.
	virtual void fixUp() OVERRIDE;

	// Caches all the resources metadata.
	ResourcesCache resources;
	ResourcesCacheMap resourcesCache;
};

//-----------------------------------//

} } // end namespaces