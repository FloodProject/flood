/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"
#include "Core/Archive.h"

NAMESPACE_BEGIN

//-----------------------------------//

bool ArchiveOpen(Archive* archive, const String& path)
{
	if( !archive ) return false;
	return archive->fn->open(archive, path);
}

//-----------------------------------//

void ArchiveDestroy(Archive* archive, Allocator* alloc)
{
	if( !archive ) return;
	archive->fn->close(archive);
	Deallocate(alloc, archive);
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

NAMESPACE_END