/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

#include "vapor/resources/ResourceFile.h"

namespace vapor { namespace resources {

//-----------------------------------//

ResourceFile::ResourceFile( vfs::FilePtr file )
	: file( file )
{

}

//-----------------------------------//

ResourceFile::~ResourceFile()
{

}

//-----------------------------------//

const std::string& ResourceFile::getURI() const
{
	return file->getPath();
}

//-----------------------------------//

} } // end namespaces