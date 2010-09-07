/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/CompileOptions.h"

#ifdef VAPOR_MESH_MILKSHAPE3D

#include "vapor/resources/ResourceLoader.h"
#include "vapor/resources/Milkshape3D.h"

namespace vapor {

//-----------------------------------//

/**
 * Loads a given Milkshape3D mesh format.
 */

class Milkshape3D_Loader : public ResourceLoader
{
public:

	Milkshape3D_Loader();

	// Creates the resource with no data.
	PREPARE(Milkshape3D)

	// Decodes a Milkshape3D mesh.
	virtual bool decode(const File&, Resource* res);

	// Gets the name of this codec.
	GETTER(Name, const std::string, "Milkshape3D")

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