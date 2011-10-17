/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"
#include "Core/Archive.h"

NAMESPACE_CORE_BEGIN

//-----------------------------------//

Archive::Archive()
	: handle(nullptr)
	, userdata(nullptr)
	, watchId(0)
	, fn(nullptr)
{
}

//-----------------------------------//

Archive::~Archive()
{
}

//-----------------------------------//

bool ArchiveOpen(Archive* archive, const String& path)
{
	if( !archive ) return false;
	return archive->fn->open(archive, path);
}

//-----------------------------------//

void ArchiveDestroy(Archive* archive)
{
	if( !archive ) return;
	archive->fn->close(archive);
	Deallocate(archive);
}

//-----------------------------------//

Stream* ArchiveOpenFile(Archive* archive, const Path& path, Allocator* alloc)
{
	if( !archive ) return nullptr;
	return archive->fn->open_file(archive, path, alloc);
}

//-----------------------------------//

bool ArchiveExistsFile(Archive* archive, const Path& path)
{
	if( !archive ) return false;
	Path normalized = PathNormalize(path);
	return archive->fn->exists_file(archive, normalized);
}

//-----------------------------------//

bool ArchiveExistsDirectory(Archive* archive, const Path& path)
{
	if( !archive ) return false;
	Path normalized = PathNormalize(path);
	return archive->fn->exists_dir(archive, normalized);
}

//-----------------------------------//

void ArchiveEnumerateFiles(Archive* archive, std::vector<String>& files)
{
	if( !archive ) return;
	archive->fn->enumerate_files(archive, files);
}

//-----------------------------------//

void ArchiveEnumerateDirectories(Archive* archive, std::vector<String>& dirs)
{
	if( !archive ) return;
	archive->fn->enumerate_dirs(archive, dirs);
}

//-----------------------------------//

bool ArchiveWatchUpdate(Archive* archive)
{
	if( !archive ) return false;
	return archive->fn->watch(archive);
}

//-----------------------------------//

Path ArchiveCombinePath(Archive* archive, const Path& filePath)
{
	if( !archive ) return "";
	
	const Path& sep = PathGetSeparator();
	Path fullPath = StringFormat("%s%s%s", archive->path.c_str(), sep.c_str(), filePath.c_str());

	return PathNormalize(fullPath);
}

//-----------------------------------//

NAMESPACE_CORE_END