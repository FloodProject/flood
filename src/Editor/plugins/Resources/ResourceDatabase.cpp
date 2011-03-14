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
#include "Utilities.h"
#include "io/JsonSerializer.h"

namespace vapor { namespace editor {

//-----------------------------------//

REFLECT_CLASS(ResourceMetadata)
	FIELD_PRIMITIVE(uint, hash)
	FIELD_PRIMITIVE(string, path)
REFLECT_END()

REFLECT_CLASS(ResourceDatabase)
	FIELD_VECTOR(ResourceMetadata, resources)
REFLECT_END()

//-----------------------------------//

ResourceDatabase::ResourceDatabase()
{
	loadCache();
}

//-----------------------------------//

ResourceDatabase::~ResourceDatabase()
{
	saveCache();
}

//-----------------------------------//

bool ResourceDatabase::loadCache()
{
#if 0
	LocaleSwitch locale;
	std::string path = CacheFolder + ThumbCache;

	if( !wxFileName::DirExists(CacheFolder) )
	{
		LogInfo("Creating cache directory: '%s'", CacheFolder.c_str());
		wxFileName::Mkdir(CacheFolder);
		return false;
	}

	if( !File::exists(path) )
	{
		LogWarn("Could not find thumbnails cache file '%s'", path.c_str());
		return false;
	}

	FileStream file( path, StreamMode::Read );

	if( !file.open() )
	{
		LogWarn("Could not open thumbnails cache file '%s'", path.c_str());
		return false;
	}

	std::string text;
	file.read(text);

	Json::Value root;
	Json::Reader reader;
	
	if( !reader.parse(text, root, false) )
		return false;

	uint i;
	for( i = 0; i < root.size(); i++ )
	{
		if( !root.isValidIndex(i) )
			continue;

		const Json::Value& value = root[i];

		if( value.isNull() || value.empty() )
			continue;

		if( !value.isMember("hash") || !value.isMember("thumb") )
			continue;

		const Json::Value& valHash = value["hash"];
		const Json::Value& valThumb = value["thumb"];

		if( valHash.isNull() || valHash.empty() )
			continue;

		if( valThumb.isNull() || valThumb.empty() )
			continue;

		ResourceMetadata metadata;
		metadata.hash = valHash.asUInt();
		metadata.thumbnail = valThumb.asString();

		resources.push_back(metadata);
		resourcesCache[metadata.hash] = metadata;
	}

	LogInfo("Loaded thumbnails cache from '%s' with %u entries", path.c_str(), i);

	return true;
#endif

	return false;
}

//-----------------------------------//

bool ResourceDatabase::saveCache()
{
#if 0
	LocaleSwitch locale;
	std::string path = CacheFolder + ThumbCache;
	FileStream file( path, StreamMode::Write );

	if( !file.open() )
		return false;
	
	Json::Value root;
	uint i = 0;

	ResourcesCacheMap::const_iterator it;
	
	for( it = resourcesCache.begin(); it != resourcesCache.end(); it++ )
	{
		const ResourceMetadata& metadata = it->second;
		
		Json::Value value;
		value["hash"] = metadata.hash;
		value["thumb"] = metadata.thumbnail;
		
		root[i++] = value;
	}

	file.write( root.toStyledString() );
	LogInfo("Wrote thumbnails cache to '%s' with %u entries", path.c_str(), i);

	return true;
#endif

	return false;
}

//-----------------------------------//

void ResourceDatabase::fixUp()
{
	for( uint i = 0; i < resources.size(); i++ )
	{
		const ResourceMetadata& metadata = resources[i];
		resourcesCache[metadata.hash] = metadata;
	}
}

//-----------------------------------//

void ResourceDatabase::scanFiles()
{
	ResourceManager* rm = GetResourceManager();

	std::vector<std::string> found;	
	FileEnumerateFiles("media/meshes", found);
	
	std::vector<std::string> files;	
	for( size_t i = 0; i < found.size(); i++ )
	{
		const std::string& path = found[i];

		std::string ext = PathGetFileExtension(path);
		ResourceLoader* loader = rm->findLoader(ext);

		if( !loader ) continue;
		if( loader->getResourceGroup() != ResourceGroup::Meshes ) continue;

		File file(path, StreamMode::Read);
		
		std::vector<byte> data;
		FileRead(&file, data);

		FileClose(&file);

		uint hash = Hash::Murmur2( data, 0xBEEF );
		
		if( resourcesCache.find(hash) != resourcesCache.end() )
			continue;

		files.push_back(path);
	}

#if 0
	if( !files.empty() )
		generateThumbnails(files);
#endif
}

//-----------------------------------//
#if 0
void ResourceDatabase::generateThumbnails(const std::vector<std::string>& files)
{
	ResourceManager* res = GetResourceManager();

	wxProgressDialog progressDialog( "Loading resources",
		"Please wait while resources are loaded.", files.size(),
		this, wxPD_AUTO_HIDE | wxPD_SMOOTH | wxPD_CAN_ABORT );
	
	progressDialog.Show();

	uint progress = 0;

	for( uint i = 0; i < files.size(); i++ )
	{
		const std::string& path = files[i];
		
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

		const std::string& resPath = PathUtils::getFile(mesh->getPath());

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