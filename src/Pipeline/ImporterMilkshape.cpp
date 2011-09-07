/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Pipeline/API.h"

#ifdef ENABLE_IMPORTER_MILKSHAPE

#include "Pipeline/ImporterMilkshape.h"
#include "Pipeline/Milkshape3D.h"
#include "Resources/Skeleton.h"

NAMESPACE_RESOURCES_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(ImporterMilkshape, ResourceImporter)
REFLECT_CLASS_END()

//-----------------------------------//

ImporterMilkshape::ImporterMilkshape()
{
	extensions.push_back("ms3d");
}

//-----------------------------------//

bool ImporterMilkshape::decode(const Stream& stream, Resource* res)
{
	Mesh* mesh = static_cast<Mesh*>(res);
	
	Milkshape3D milkshape;
	milkshape.setMesh(mesh);

	return milkshape.load(stream);
}

//-----------------------------------//

NAMESPACE_RESOURCES_END

#endif