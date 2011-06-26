/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Editor/API.h"
#include "ResourceDatabase.h"
#include "Settings.h"
#include "Core/Utilities.h"

namespace vapor { namespace editor {

//-----------------------------------//

REFLECT_CLASS(ResourceMetadata)
	FIELD_PRIMITIVE(uint32, hash)
	FIELD_PRIMITIVE(string, path)
REFLECT_CLASS_END()

REFLECT_CHILD_CLASS(ResourceDatabase, Object)
	FIELD_VECTOR(ResourceMetadata, resources)
REFLECT_CLASS_END()

//-----------------------------------//

ResourceDatabase::ResourceDatabase()
{
	scanFiles();
	indexFiles();
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

void ResourceDatabase::scanFiles()
{
	std::vector<String> found;	
	FileEnumerateFiles(MediaFolder, found);
	
	for( size_t i = 0; i < found.size(); i++ )
	{
		resourcesToIndex.Post(found[i]);
	}
}

//-----------------------------------//

void ResourceDatabase::indexFiles()
{
	ResourceManager* rm = GetResourceManager();

	String path;
	while( resourcesToIndex.ReceiveTimeout(0, path) == wxMSGQUEUE_NO_ERROR )
	{
		LogDebug("Indexing file '%s'", path.c_str());

		String ext = PathGetFileExtension(path);
		
		ResourceLoader* loader = rm->findLoader(ext);
		if( !loader ) continue;
	
		Stream* file = StreamCreateFromFile(AllocatorGetHeap(), MediaFolder+path, StreamMode::Read);
		
		if( !file )
		{
			LogWarn("Could not read file '%s'", path.c_str());
			continue;
		}

		std::vector<byte> data;
		StreamRead(file, data);
		StreamDestroy(file);

		uint32 hash = Hash::Murmur2( data, 0xBEEF );
		
		if( resourcesCache.find(hash) != resourcesCache.end() )
			continue;

		ResourceMetadata metadata;
		metadata.hash = hash;
		metadata.path = path;
		metadata.group = loader->getResourceGroup();

		resourcesCache[hash] = metadata;

		resourcesToThumb.Post(path);
	}
}

//-----------------------------------//
#if 0

void ResourceDatabase::generateThumbnails(const std::vector<String>& files)
{
	ResourceManager* res = GetResourceManager();

	wxProgressDialog progressDialog( "Loading resources",
		"Please wait while resources are loaded.", files.size(),
		this, wxPD_AUTO_HIDE | wxPD_SMOOTH | wxPD_CAN_ABORT );
	
	progressDialog.Show();

	size_t progress = 0;

	for( size_t i = 0; i < files.size(); i++ )
	{
		const String& path = files[i];
		
		// Force unused resources to be unloaded.
		res->update(0);

		progressDialog.Update(progress++, PathUtils::getFile(path));

		if( progressDialog.WasCancelled() )
			break;

		File file(path);
		
		std::vector<byte> data;
		file.read(data);

		uint hash = Hash::Murmur2( data, 0xBEEF );
		
		if( resourcesCache.find(hash) != resourcesCache.end() )
			continue;

		ResourceLoadOptions options;
		options.name = path;
		options.asynchronousLoading = false;

		MeshPtr mesh = RefCast<Mesh>(res->loadResource(options));

		if( !mesh || mesh->getResourceGroup() != ResourceGroup::Meshes )
			continue;

		const String& resPath = PathGetFile(mesh->getPath());

		ResourceMetadata metadata;
		metadata.hash = hash;
		metadata.thumbnail = resPath + ".png";
		metadata.path = resPath;
		resourcesCache[hash] = metadata;

		ImagePtr thumb = generateThumbnail(mesh);

		if( !thumb )
			continue;

		ImageWriter writer;
		writer.save( thumb, CacheFolder + metadata.thumbnail );

		LogInfo("Generated thumbnail for resource '%s'", resPath.c_str());
	}
}
#endif

//-----------------------------------//

} } // end namespaces