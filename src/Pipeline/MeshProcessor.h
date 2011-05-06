/************************************************************************
*
* vapor3D © (2008-2011)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "ResourceProcessor.h"
#include "Resources/Mesh.h"
#include "Resources/Animation.h"
#include "Resources/Skeleton.h"

namespace vapor {

//-----------------------------------//

/**
 * Resource processors handle the processing of resource data.
 */

REFLECT_DECLARE_CLASS(MeshProcessor)

class PIPELINE_API MeshProcessor : public ResourceProcessor
{
	REFLECT_DECLARE_OBJECT(MeshProcessor)

public:

	MeshProcessor();

	// Processes the given resource.
	bool Process(const ResourcePtr& resource);

	// Gets the processed resource type.
	Class* GetResourceType() { return MeshGetType(); }

private:
};

//-----------------------------------//

} // end namespace
