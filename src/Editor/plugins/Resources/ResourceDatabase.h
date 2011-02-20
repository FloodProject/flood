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

struct ResourceMetadata
{
	REFLECT_DECLARE_CLASS()

	// Hash of the resource.
	uint hash;

	// Path of the resource.
	std::string path;

	// Thumbnail of the resource.
	std::string thumbnail;
};

typedef std::vector<ResourceMetadata> ResourcesCache;
typedef std::map<uint, ResourceMetadata> ResourcesCacheMap;
typedef std::pair<uint, ResourceMetadata> ResourcesCacheMapPair;

//-----------------------------------//

class ResourceDatabase : public Object
{
	REFLECT_DECLARE_CLASS()

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
	virtual void fixUp();

	// Caches all the resources metadata.
	ResourcesCache resources;
	ResourcesCacheMap resourcesCache;
};

//-----------------------------------//

} } // end namespaces