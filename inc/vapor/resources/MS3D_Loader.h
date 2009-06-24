/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/CompileOptions.h"

#ifdef VAPOR_MESH_MILKSHAPE3D

#include "vapor/resources/ResourceLoader.h"
#include "vapor/resources/MS3D.h"

namespace vapor {
	namespace resources {

/**
 * Loads a given MS3D mesh format.
 */

class MS3D_Loader : public ResourceLoader
{
public:

	MS3D_Loader();

	// Decode an .ms3d mesh file
	virtual MS3D* decode(vfs::File& file);

	// Gets the name of this codec.
	virtual string getName() { return "MS3D"; }

	// Gets the list of extensions recognized by this resource handler.
	virtual list<string>& getExtensions() { return extensions; }

	// Returns the right resource group for this type of resource
	virtual ResourceGroup::Enum getResourceGroup() { return ResourceGroup::Meshes; }

protected:

	// holds all file extensions recognized by this codec
	list<string> extensions;
};

} } // end namespaces

#endif