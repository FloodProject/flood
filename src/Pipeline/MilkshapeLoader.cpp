/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Pipeline/API.h"

#ifdef VAPOR_MESH_MILKSHAPE3D

#include "Pipeline/MilkshapeLoader.h"
#include "Pipeline/Milkshape3D.h"
#include "Resources/Skeleton.h"

namespace vapor {

//-----------------------------------//

BEGIN_CLASS_PARENT(MilkshapeLoader, ResourceLoader)
END_CLASS()

//-----------------------------------//

MilkshapeLoader::MilkshapeLoader()
{
	extensions.push_back("ms3d");
}

//-----------------------------------//

bool MilkshapeLoader::decode(const Stream& stream, Resource* res)
{
	Mesh* mesh = static_cast<Mesh*>(res);
	
	Milkshape3D milkshape;
	milkshape.setMesh(mesh);

	if( !milkshape.load(stream) )
		return false;
	
	return true;
}

//-----------------------------------//

} // end namespace

#endif