/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"
#include "FileStream.h"
#include <cassert>

#ifdef VAPOR_PLATFORM_WINDOWS
	#include <io.h>
	#define F_OK 0
#else
	#include <unistd.h>
#endif

#ifdef VAPOR_COMPILER_MSVC
	#define access _access
#endif

namespace vapor {

//-----------------------------------//

FileStream::FileStream(const String& path, StreamMode::Enum mode)
  : Stream(mode, path)
  , fp(nullptr)
{ }

//-----------------------------------//

FileStream::~FileStream() 
{
	close();
}

//-----------------------------------//

bool FileStream::open()
{
	const char* mode_ = nullptr;

	if( mode == StreamMode::Read )
		mode_ = "rb";
	else if( mode == StreamMode::Write )
		mode_ = "w+b";
	else if( mode == StreamMode::Append )
		mode_ = "a+b";

#ifdef VAPOR_COMPILER_MSVC
	fopen_s(&fp, path.c_str(), mode_);
#else
	fp = fopen(path.c_str(), mode_);
#endif

	if ( !fp ) return false;

	return true;
}

//-----------------------------------//

void FileStream::close()
{
	if(!fp) return;
	
	fclose(fp);
	fp = nullptr;
}

//-----------------------------------//

int FileStream::tell()
{
	if(!fp) return -1;
	
	return ftell(fp);
}

//-----------------------------------//

long FileStream::read(void* buffer, long size) const
{
	return fread(buffer, 1, size, fp);
}

//-----------------------------------//

void FileStream::read(std::vector<byte>& data) const
{
	long size = getSize();

	data.resize(size);

	if(size > 0)
		read(&data.front(), size);
}

//-----------------------------------//

long FileStream::write(const std::vector<byte>& buf)
{
	assert( mode == StreamMode::Write || mode == StreamMode::Append );
	assert( !buf.empty() );

	if( !fp || buf.empty() ) return -1;

	return fwrite(&buf.front(), buf.size(), 1, fp);  
}

//-----------------------------------//

bool FileStream::exists() const
{
	return access(path.c_str(), F_OK) == 0;
}

//-----------------------------------//

bool FileStream::exists(const String& path)
{
	return access(path.c_str(), F_OK) == 0;
}

//-----------------------------------//

void FileStream::setBuffering( bool state )
{
	int mode = _IOFBF;
	
	if( !state )
		mode = _IONBF;
	
	setvbuf(fp, nullptr, mode, 0);
}

//-----------------------------------//

long FileStream::getSize() const
{
	// Hold the current file position.
	long curr = ftell(fp);
	
	// Seek to the end of the file and get position.
	fseek(fp, 0, SEEK_END);
	long size = ftell(fp);
	
	// Seek again to the previously current position.
	fseek(fp, curr, SEEK_SET);

	return size;
}

//-----------------------------------//

FILE* FileStream::getFilePointer()
{
	return fp;
}

//-----------------------------------//

} // end namespace

#undef access