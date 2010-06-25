/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/NativeFile.h"

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

NativeFile::NativeFile(const std::string& path, AccessMode::Enum mode)
  : path(path),
	mode(mode),
	fp(nullptr)
{ }

//-----------------------------------//

NativeFile::NativeFile(const char* path, AccessMode::Enum mode)
  : path(path),
	mode(mode),
	fp(nullptr)
{ }

//-----------------------------------//

NativeFile::~NativeFile() 
{
	close();
}

//-----------------------------------//

bool NativeFile::open()
{
	char* mode_ = nullptr;

	if( mode == AccessMode::Read )
		mode_ = "rb";
	else if( mode == AccessMode::Write )
		mode_ = "w+b";
	else if( mode == AccessMode::Append )
		mode_ = "a+b";

	fp = fopen(path.c_str(), mode_);
	
	if ( !fp ) 
		return false;

	return true;
}

//-----------------------------------//

void NativeFile::close()
{
	if(!fp)
		return;
	
	fclose(fp);
	fp = nullptr;
}

//-----------------------------------//

void NativeFile::setBuffering( bool state )
{
	if( !state )
	{
		// Turn off file buffering.
		setbuf(fp, nullptr);
	}
	else
	{
		assert( 0 && "Not implemented" );
	}
}

//-----------------------------------//

long NativeFile::getSize()
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

long NativeFile::read(void* buffer, long size)
{
	return fread(buffer, 1, size, fp);
}

//-----------------------------------//

long NativeFile::write(const std::string& text)
{
	std::vector<byte> data( text.begin(), text.end() );
	return write(data);  
}

//-----------------------------------//

long NativeFile::write(const std::vector<byte>& buf)
{
	assert( !buf.empty() );

	if( buf.empty() )
		return -1;

	return fwrite(&buf.front(), buf.size(), 1, fp);  
}

//-----------------------------------//

bool NativeFile::exists()
{
	return access(path.c_str(), F_OK) == 0;
}

//-----------------------------------//

bool NativeFile::exists(const std::string& path)
{
	return access(path.c_str(), F_OK) == 0;
}

//-----------------------------------//

} // end namespace

#undef access