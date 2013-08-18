/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Pipeline/API.h"

#ifdef ENABLE_IMPORTER_MILKSHAPE

#include "Pipeline/ImporterMilkshape.h"
#include "Pipeline/Milkshape3D.h"
#include "Engine/Resources/Skeleton.h"
#include "Core/Array.h"

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

ExtensionMetadata* ImporterMilkshape::getMetadata()
{
	static ExtensionMetadata s_ExtensionMetadata =
	{
		"Milkshape3D",
		"Imports a MS3D mesh."
	};

	return &s_ExtensionMetadata;
}

//-----------------------------------//

bool ImporterMilkshape::decode(ResourceLoadOptions& options)
{
	Mesh* mesh = static_cast<Mesh*>(options.resource);
	
	Milkshape3D milkshape;
	milkshape.setMesh(mesh);

	return milkshape.load(*options.stream);
}

//-----------------------------------//

NAMESPACE_RESOURCES_END

#endif