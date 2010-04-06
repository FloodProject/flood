/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/resources/Resource.h"
#include "vapor/scene/Geometry.h"

namespace vapor { namespace resources {

//-----------------------------------//

/**
 * Mesh resources contain 3D geometry data.
 */

class Mesh : public Resource
{
public:
	
	// Gets the resource group of this resource.
	IMPLEMENT_GETTER(ResourceGroup, ResourceGroup::Enum, ResourceGroup::Meshes)

	// Gets the geometry for this mesh resource.
	virtual scene::GeometryPtr getGeometry() = 0;

private:

	static const std::string& type;
};

//-----------------------------------//

TYPEDEF_RESOURCE_POINTER_FROM_TYPE( Mesh );

//-----------------------------------//

} } // end namespaces