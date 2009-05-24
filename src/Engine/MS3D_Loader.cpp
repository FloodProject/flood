/************************************************************************
*
* vaporEngine by triton © (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/CompileOptions.h"

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

Milkshape3D* MS3D_Loader::decode(File& mesh)
{
	return nullptr;
}

//-----------------------------------//

} } // end namespaces

#endif