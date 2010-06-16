/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/resources/Resource.h"

FWD_DECL_TYPEDEF_INT(Renderable)

namespace vapor {

//-----------------------------------//

/**
 * Mesh resources contain 3D geometry data.
 */

class Mesh : public Resource
{
public:

	// Builds the geometry of the mesh.
	virtual void build( std::vector<RenderablePtr>& renderables ) = 0;
	
	// Gets if the mesh is built.
	bool isBuilt() const;

	// Gets the resource group of this resource.
	GETTER(ResourceGroup, ResourceGroup::Enum, ResourceGroup::Meshes)

protected:

	Mesh();

	bool built;
};

TYPEDEF_RESOURCE_POINTER_FROM_TYPE( Mesh );

//-----------------------------------//

} // end namespace