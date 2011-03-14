/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"
#include "Core/File.h"
#include "Core/Memory.h"

#ifdef VAPOR_VFS_PHYSFS

#include "Log.h"
#include "Utilities.h"
#include <dirent.h>

#include <physfs.h>

namespace vapor {

//-----------------------------------//

File::File()
	: Handle(nullptr)
	, IsClosed(false)
{ }

//-----------------------------------//

File::File(const String& path, StreamMode::Enum mode)
	: Handle(nullptr)
	, Path(path)
	, Mode(mode)
	, IsClosed(false)
{
	FileOpen(this);
}

//-----------------------------------//

File* FileCreate(MemoryAllocator* mem, const Path& path, StreamMode::Enum mode)
{
	File* file = Allocate<File>(mem);

	file->Handle = nullptr;
	file->Mode = mode;
	file->IsClosed = false;

	Path newPath = PathNormalize(path);
	newPath = PathGetFile(newPath);
	file->Path = newPath;

	FileOpen(file);

	return file;
}

//-----------------------------------//

void FileDestroy(File* file, MemoryAllocator* mem)
{
	Deallocate(mem, file);
}

//-----------------------------------//

static void FileLog(File* file, const String& err)
{
	LogError( "%s '%s': %s", err.c_str(), file->Path.c_str(), PHYSFS_getLastError() );
}

//-----------------------------------//

bool FileOpen(File* file)
{
	if( !file ) return false;
	
	assert( file->Handle == 0 );
	const char* path = file->Path.c_str();

	switch(file->Mode)
	{
	case StreamMode::Read:
		file->Handle = PHYSFS_openRead(path);
		break;
	case StreamMode::Write:
		file->Handle = PHYSFS_openWrite(path);
		break;
	case StreamMode::Append:
		file->Handle = PHYSFS_openAppend(path);
		break;
	}

	if( !file->Handle )
	{
		FileLog(file, "Could not open file");
		return false;
	}

	return true;
}

//-----------------------------------//

bool  FileClose(File* file)
{
	if( !file ) return false;
	if( file->IsClosed ) return true;
	
	if( !PHYSFS_close(file->Handle) )
		return false;

	file->Handle = nullptr;
	file->IsClosed = true;

	return true;
}

//-----------------------------------//

sint64 FileGetSize(File* file)
{
	if( !file ) return 0;

	PHYSFS_sint64 size = PHYSFS_fileLength(file->Handle);
	return size;
}

//-----------------------------------//

static void FileValidate(File* file, StreamMode::Enum mode)
{
	assert( file->Mode == mode );
}

//-----------------------------------//

bool FileReadBuffer(File* file, void* data, sint32 size)
{
	if( !file ) return false;

#if 0
	if( !FileValidate(file, StreamMode::Read) ) return false;
	if( !FileValidate(file, StreamMode::Append) ) return false;
#endif

	if( PHYSFS_eof(file->Handle) ) return false; 

	sint64 read = PHYSFS_read(file->Handle, data, 1, size);

	if(read < 0)
	{
		FileLog(file, "Could not read from file");
		return false;	
	}

	return true;
}

//-----------------------------------//

bool FileRead(File* file, std::vector<uint8>& data)
{
	if( !file ) return false;

	sint32 size = sint32(FileGetSize(file) - FileTell(file));
	if( size == 0 ) return false;

	data.resize(size); 

	return FileReadBuffer(file, &data[0], data.size());
}

//-----------------------------------//

bool FileReadLines(File* file, std::vector<String>& lines)
{
	if( !file ) return false;

	std::vector<byte> data;
	FileRead(file, data);

	String text( data.begin(), data.end() );
	StringSplit(text, '\n', lines);
	
	// Remove extra newlines.
	for( size_t i = 0; i < lines.size(); i++ )
	{
		String& line = lines[i];
		size_t end = line.size() - 1;
		if( line[end] == '\r' ) line.erase(end);
	}

	return true;
}

//-----------------------------------//

sint64 FileWrite(File* file, const std::vector<byte>& data)
{
	if( !file ) return 0;

#if 0
	if( !FileValidate(file, StreamMode::Write) ) return 0;
#endif

	sint64 written = PHYSFS_write(file->Handle, &data[0], 1, data.size());
	
	if(written < 0)
	{
		FileLog(file, "Could not write to file");
		return 0;
	}

	return written;
}

//-----------------------------------//

sint64 FileWriteString(File* file, const String& text)
{
	std::vector<uint8> data( text.begin(), text.end() );
	return FileWrite(file, data);
}

//-----------------------------------//

bool FileSeek(File* file, sint64 pos)
{
	if( !file ) return false;
	
	if(pos < 0 || pos >= FileGetSize(file))
	{
		FileLog(file, "Seek to out of bounds position");
		return false;
	}
	
	if( !PHYSFS_seek(file->Handle, pos) )
	{
		FileLog(file, "Failure to seek");
		return false;
	}

	return true;
}

//-----------------------------------//

sint64 FileTell(File* file)
{
	if( !file ) return 0;

	PHYSFS_sint64 position = PHYSFS_tell(file->Handle);
	return position;
}

//-----------------------------------//

Path FileGetFullPath(File* file)
{
	Path fullPath( PHYSFS_getRealDir(file->Path.c_str()) );
	fullPath.append( PathGetSeparator() );
	fullPath.append( file->Path );

	return PathNormalize(fullPath);
}

//-----------------------------------//

bool FileExists(const Path& path)
{
	return PHYSFS_exists(path.c_str()) != 0;
}

//-----------------------------------//

static void EnumerateHelper(std::vector<String>& files, const String& path, bool dirs)
{
	DIR *dir;
	struct dirent *ent;

	// Open directory stream.
	dir = opendir( path.c_str() );
    
	if( !dir ) return;

	// Get all the files and directories within directory.
	while((ent = readdir(dir)) != nullptr)
	{
		String name = ent->d_name;
		String newPath = path + "/" + name;

		switch(ent->d_type) {
		case DT_REG:
		{
			if(!dirs) files.push_back(newPath);
			break;
		}
		case DT_DIR:
		{
			if(!name.empty() && name[0] == '.') continue;
			if(dirs) files.push_back(newPath);
			EnumerateHelper(files, newPath, dirs);
			break;
		} }
	}

	closedir(dir);
}

//-----------------------------------//

void FileEnumerateFiles(const String& path, std::vector<String>& files)
{
	EnumerateHelper(files, path, false);
}

//-----------------------------------//

void FileEnumerateDirs(const String& path, std::vector<String>& dirs)
{
	EnumerateHelper(dirs, path, true);
}

//-----------------------------------//

} // end namespace

#endif