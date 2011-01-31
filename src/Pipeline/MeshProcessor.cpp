/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Pipeline/API.h"
#include "Pipeline/MeshProcessor.h"

namespace vapor {

//-----------------------------------//

BEGIN_CLASS_PARENT(MeshProcessor, ResourceProcessor)
END_CLASS()

//-----------------------------------//

MeshProcessor::MeshProcessor()
{ }

//-----------------------------------//

bool MeshProcessor::Process(const ResourcePtr& resource)
{
	return true;
}

//-----------------------------------//

} // end namespace