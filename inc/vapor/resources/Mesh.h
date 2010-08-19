/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/resources/Resource.h"

FWD_DECL_INTRUSIVE(Renderable)

namespace vapor {

//-----------------------------------//

/**
 * Mesh resources contain 3D geometry data.
 */

class Mesh : public Resource
{
	DECLARE_CLASS_()

public:

	// Builds the geometry of the mesh.
	virtual void build() = 0;
	
	// Gets if the mesh is built.
	bool isBuilt() const;

	// Gets the renderables of the mesh.
	void appendRenderables( std::vector<RenderablePtr>& renderables );

	// Gets the resource group of this resource.
	GETTER(ResourceGroup, ResourceGroup::Enum, ResourceGroup::Meshes)

protected:

	Mesh();

	// Keeps track if the mesh has been built.
	bool built;

	// Renderables of the mesh.
	std::vector<RenderablePtr> renderables;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Mesh );

//-----------------------------------//

} // end namespace