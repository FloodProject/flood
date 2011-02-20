/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#ifdef VAPOR_MESH_MILKSHAPE3D

#include "Resources/ResourceLoader.h"
#include "Resources/Mesh.h"

namespace vapor {

//-----------------------------------//

/**
 * Loads a given Milkshape3D mesh format.
 */

class MilkshapeLoader : public ResourceLoader
{
	DECLARE_CLASS_()

public:

	MilkshapeLoader();

	// Creates the resource with no data.
	RESOURCE_LOADER_PREPARE(Mesh)

	// Decodes a Milkshape3D mesh.
	virtual bool decode(const Stream& stream, Resource* res);

	// Gets the name of this codec.
	GETTER(Name, const std::string, "MS3D")

	// Overrides this to return the right resource group.
	GETTER(ResourceGroup, ResourceGroup::Enum, ResourceGroup::Meshes)
};

//-----------------------------------//

} // end namespace

#endif