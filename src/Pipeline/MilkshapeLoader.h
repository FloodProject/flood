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
	PREPARE(Mesh)

	// Decodes a Milkshape3D mesh.
	virtual bool decode(const Stream& stream, Resource* res);

	// Gets the name of this codec.
	GETTER(Name, const std::string, "MS3D")

	// Gets the list of extensions recognized by this resource handler.
	GETTER(Extensions, ExtensionList&, extensions)

	// Overrides this to return the right resource group.
	GETTER(ResourceGroup, ResourceGroup::Enum, ResourceGroup::Meshes)

protected:

	// Holds all file extensions recognized by this codec.
	mutable ExtensionList extensions;
};

//-----------------------------------//

} // end namespace

#endif