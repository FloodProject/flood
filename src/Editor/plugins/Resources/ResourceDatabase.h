/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include <wx/msgqueue.h>

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

	// Group of the resource.
	ResourceGroup::Enum group;
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

	// Scans for known resources.
	void scanFiles();

	// Indexes found resources.
	void indexFiles();

	// Serialization fix-up.
	virtual void fixUp() OVERRIDE;

	// Caches all the resources metadata.
	ResourcesCache resources;
	ResourcesCacheMap resourcesCache;

	wxMessageQueue<String> resourcesToIndex;
	wxMessageQueue<String> resourcesToThumb;
};

//-----------------------------------//

} } // end namespaces