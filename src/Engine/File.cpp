/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/vfs/File.h"

#ifdef VAPOR_PLATFORM_WINDOWS
	#include <io.h>
	#define F_OK 0
#else
	#include <unistd.h>
#endif

#ifdef VAPOR_PLATFORM_WINDOWS
	// disable Visual C++ fopen deprecation warning
	#pragma warning(disable : 4996)
#endif

namespace vapor {
	namespace vfs {

//-----------------------------------//

File::File(string path)
	: path(path), fp(nullptr)
{
	fp = fopen(path.c_str(), "rb");
}

//-----------------------------------//

File::~File() 
{
	close();
}

//-----------------------------------//

void File::close()
{
	// close the file
	if(fp != nullptr) {
		fclose(fp);
	}
}

//-----------------------------------//

long File::getSize()
{
	// hold the current file position
	long curr = ftell(fp);
	
	// seek to the end of the file and get position
	fseek(fp, 0, SEEK_END);
	long fileSize = ftell(fp);
	
	// seek again to the previously current position
	fseek(fp, curr, SEEK_SET);

	return fileSize;
}

//-----------------------------------//

long File::read(void* buffer, long sz)
{	
	return fread(buffer, 1, sz, fp);
}

//-----------------------------------//

bool File::exists()
{
	return _access(path.c_str(), F_OK) == 0;
}

//-----------------------------------//

bool File::exists(string path)
{
	return _access(path.c_str(), F_OK) == 0;
}

//-----------------------------------//

} } // end namespaces