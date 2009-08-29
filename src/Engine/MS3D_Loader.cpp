/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

#ifdef VAPOR_MESH_MILKSHAPE3D

#include "vapor/resources/MS3D_Loader.h"

using vapor::vfs::File;

namespace vapor {
	namespace resources {

//-----------------------------------//

MS3D_Loader::MS3D_Loader()
{
	extensions.push_back("ms3d");
}

//-----------------------------------//

MS3D* MS3D_Loader::decode(File& file)
{
	if(!file.exists()) return nullptr;

	MS3D* mesh = new MS3D(file.getPath());
	return mesh;
}

//-----------------------------------//

} } // end namespaces

#endif