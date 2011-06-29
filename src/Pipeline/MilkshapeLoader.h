/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#ifdef ENABLE_MESH_MILKSHAPE3D

#include "Resources/ResourceLoader.h"
#include "Resources/Mesh.h"

namespace vapor {

//-----------------------------------//

/**
 * Loads a given Milkshape3D mesh format.
 */

REFLECT_DECLARE_CLASS(MilkshapeLoader)

class MilkshapeLoader : public ResourceLoader
{
	REFLECT_DECLARE_OBJECT(MilkshapeLoader)

public:

	MilkshapeLoader();

	// Creates the resource with no data.
	RESOURCE_LOADER_PREPARE(Mesh)

	// Gets the class of the resource.
	RESOURCE_LOADER_CLASS(Mesh)

	// Decodes a Milkshape3D mesh.
	virtual bool decode(const Stream& stream, Resource* res) OVERRIDE;

	// Gets the name of this codec.
	GETTER(Name, const String, "MS3D")

	// Overrides this to return the right resource group.
	GETTER(ResourceGroup, ResourceGroup::Enum, ResourceGroup::Meshes)
};

//-----------------------------------//

} // end namespace

#endif