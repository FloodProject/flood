/************************************************************************
*
* vapor3D © (2008-2011)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "ReferenceCount.h"
#include "Reflection.h"

FWD_DECL_INTRUSIVE(Resource)

namespace vapor {

//-----------------------------------//

/**
 * Resource processors handle the pre-processing of resource data.
 */

class PIPELINE_API ResourceProcessor
{
	DECLARE_UNCOPYABLE(ResourceProcessor)
	DECLARE_CLASS_()

public:

	// Processes the given resource.
	virtual bool Process(const ResourcePtr& resource) = 0;

protected:
	
	ResourceProcessor();
};

//-----------------------------------//

} // end namespace
