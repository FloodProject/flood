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
#include "vapor/resources/MS3D.h"

namespace vapor { namespace resources {

//-----------------------------------//

/**
 * Loads a given MS3D mesh format.
 */

class MS3D_Loader : public ResourceLoader
{
public:

	MS3D_Loader();

	// Creates the resource with no data.
	IMPLEMENT_PREPARE(MS3D)

	// Decodes a Milkshape3D mesh.
	virtual bool decode(const vfs::File&, Resource* res);

	// Gets the name of this codec.
	IMPLEMENT_GETTER(Name, const std::string, "MS3D")

	// Gets the list of extensions recognized by this resource handler.
	IMPLEMENT_GETTER(Extensions, ExtensionList&, extensions)

	// Overrides this to return the right resource group.
	IMPLEMENT_GETTER(ResourceGroup, ResourceGroup::Enum, ResourceGroup::Meshes)

protected:

	// holds all file extensions recognized by this codec
	mutable ExtensionList extensions;
};

//-----------------------------------//

} } // end namespaces

#endif