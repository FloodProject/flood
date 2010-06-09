/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/resources/Resource.h"

FWD_DECL_NS_TYPEDEF_INT(render, Renderable)

namespace vapor { namespace resources {

//-----------------------------------//

/**
 * Mesh resources contain 3D geometry data.
 */

class Mesh : public Resource
{
public:

	// Builds the geometry of the mesh.
	virtual void build( std::vector<render::RenderablePtr>& renderables ) = 0;
	
	// Gets if the mesh is built.
	bool isBuilt() const;

	// Gets the resource group of this resource.
	IMPLEMENT_GETTER(ResourceGroup, ResourceGroup::Enum, ResourceGroup::Meshes)

protected:

	Mesh();

	bool built;
};

TYPEDEF_RESOURCE_POINTER_FROM_TYPE( Mesh );

//-----------------------------------//

} } // end namespaces