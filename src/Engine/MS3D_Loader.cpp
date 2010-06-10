/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"

#ifdef VAPOR_MESH_MILKSHAPE3D

#include "vapor/resources/MS3D_Loader.h"

using vapor::File;

namespace vapor {

//-----------------------------------//

MS3D_Loader::MS3D_Loader()
{
	extensions.push_back("ms3d");
}

//-----------------------------------//

bool MS3D_Loader::decode(const File& file, Resource* res)
{
	if( !file.exists() ) 
		return false;

	MS3D* mesh = static_cast<MS3D*>( res );
	
	if( !mesh->load(file) )
		return false;
	
	return true;
}

//-----------------------------------//

} // end namespace

#endif