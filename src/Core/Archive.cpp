/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"
#include "Core/Archive.h"
#include "Core/Memory.h"
#include "Core/Log.h"

#ifdef VAPOR_PLATFORM_WINDOWS
	#include <io.h>
	#define F_OK 0
#else
	#include <unistd.h>
#endif

#include <dirent.h>

NAMESPACE_BEGIN

//-----------------------------------//

bool ArchiveOpen(Archive* archive, const String& path)
{
	if( !archive ) return false;
	return archive->fn->open(archive, path);
}

//-----------------------------------//

void ArchiveDestroy(Archive* archive, MemoryAllocator* mem)
{
	if( !archive ) return;
	archive->fn->close(archive);
	Deallocate(mem, archive);
}

//-----------------------------------//

bool ArchiveExistsFile(Archive* archive, const Path& path)
{
	if( !archive ) return false;
	return archive->fn->exists_file(archive, path);
}

//-----------------------------------//

bool ArchiveExistsDirectory(Archive* archive, const Path& path)
{
	if( !archive ) return false;
	return archive->fn->exists_dir(archive, path);
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

static void DirectoryArchiveEnumerate(std::vector<String>& files, const String& path, bool dirs)
{
	// Open directory stream.
	DIR *dir = opendir( path.c_str() );
   	if( !dir ) return;

	dirent* ent = nullptr;
	
	// Get all the files and directories within directory.
	while((ent = readdir(dir)))
	{
		const Path& name = ent->d_name;
		Path newPath = path + PathGetSeparator() + name;

		switch(ent->d_type)
		{
		case DT_REG:
		{
			if(!dirs) files.push_back(newPath);
			break;
		}
		case DT_DIR:
		{
			if(!name.empty() && name[0] == '.') continue;
			if(dirs) files.push_back(newPath);
			DirectoryArchiveEnumerate(files, newPath, dirs);
			break;
		}
		}
	}

	closedir(dir);
}

//-----------------------------------//

void FileEnumerateFiles(const Path& path, std::vector<Path>& files)
{
	DirectoryArchiveEnumerate(files, path, false);
}

//-----------------------------------//

void FileEnumerateDirectories(const Path& path, std::vector<Path>& dirs)
{
	DirectoryArchiveEnumerate(dirs, path, true);
}

//-----------------------------------//

#ifdef VAPOR_COMPILER_MSVC
#define access _access
#endif

bool FileExists(const Path& path)
{
	return access(path.c_str(), F_OK) == 0;
}

#undef access

//-----------------------------------//

NAMESPACE_END