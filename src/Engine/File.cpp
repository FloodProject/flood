/************************************************************************
*
* vaporEngine by triton © (2008-2009)
*
*	<http://www.portugal-a-programar.org/
*
************************************************************************/

#include "vapor/vfs/File.h"

// disable Visual C++ fopen deprecation warning
#pragma warning(disable : 4996)

namespace vapor {
	namespace vfs {

File::File(string path)
	: _path(path)
{
	_fp = fopen(path.c_str(), "rb");
}

File::~File() 
{
	close();
}

void File::close()
{
	fclose(_fp);
}

long File::getSize()
{
	long curr = ftell(_fp);
	
	fseek(_fp, 0, SEEK_END);
	long fileSize = ftell(_fp);
	
	fseek(_fp, curr, SEEK_SET);

	return fileSize;
}

long File::read(void* buffer, long sz)
{	
	return fread(buffer, 1, sz, _fp);
}

} } // end namespaces