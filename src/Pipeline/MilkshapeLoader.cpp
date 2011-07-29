/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Pipeline/API.h"

#ifdef ENABLE_MESH_MILKSHAPE3D

#include "Pipeline/MilkshapeLoader.h"
#include "Pipeline/Milkshape3D.h"
#include "Resources/Skeleton.h"

NAMESPACE_RESOURCES_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(MilkshapeLoader, ResourceLoader)
REFLECT_CLASS_END()

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

NAMESPACE_RESOURCES_END

#endif