/************************************************************************
*
* vapor3D Engine � (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"

#ifdef VAPOR_MESH_MILKSHAPE3D

#include "vapor/resources/Milkshape3D_Loader.h"
#include "vapor/resources/Mesh.h"
#include "vapor/animation/Animation.h"
#include "vapor/animation/Skeleton.h"
#include "vapor/render/Renderable.h"

using vapor::File;

namespace vapor {

//-----------------------------------//

Milkshape3D_Loader::Milkshape3D_Loader()
{
	extensions.push_back("ms3d");
}

//-----------------------------------//

bool Milkshape3D_Loader::decode(const File& file, Resource* res)
{
	if( !file.exists() ) 
		return false;

	Milkshape3D* mesh = static_cast<Milkshape3D*>(res);
	
	if( !mesh->load(file) )
		return false;
	
	return true;
}

//-----------------------------------//

} // end namespace

#endif