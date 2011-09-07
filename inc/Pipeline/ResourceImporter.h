/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Pipeline/API.h"
#include "Resources/ResourceLoader.h"

NAMESPACE_PIPELINE_BEGIN

//-----------------------------------//

/**
 * Resource importers are like resource loaders, but are intended for
 * assets that are processed and not loaded at runtime.
 */

REFLECT_DECLARE_CLASS(ResourceImporter)

class API_PIPELINE NO_VTABLE ResourceImporter : public ResourceLoader
{
	DECLARE_UNCOPYABLE(ResourceImporter)
	REFLECT_DECLARE_OBJECT(ResourceImporter)

public:

	ResourceImporter();
	virtual ~ResourceImporter();
};

//-----------------------------------//

NAMESPACE_PIPELINE_END