/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"
#include "vapor/resources/Resource.h"
#include "vapor/scene/Geometry.h"

namespace vapor { namespace resources {

//-----------------------------------//

/**
 * Mesh resources contain 3D data used to show objects in the world.
 */

class Mesh : public Resource
{
public:
	
	// Gets the resource group of this resource.
	ResourceGroup::Enum getResourceGroup() const;

	// Gets the geometry for this mesh resource.
	virtual scene::GeometryPtr getGeometry() = 0;

private:

	static const std::string& type;
};

//-----------------------------------//

TYPEDEF_RESOURCE_POINTER_FROM_TYPE( Mesh );

//-----------------------------------//

} } // end namespaces