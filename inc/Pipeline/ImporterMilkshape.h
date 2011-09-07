/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#ifdef ENABLE_IMPORTER_MILKSHAPE

#include "Pipeline/ResourceImporter.h"
#include "Resources/Mesh.h"

NAMESPACE_RESOURCES_BEGIN

//-----------------------------------//

/**
 * Loads a given Milkshape3D mesh format.
 */

REFLECT_DECLARE_CLASS(ImporterMilkshape)

class ImporterMilkshape : public ResourceImporter
{
	REFLECT_DECLARE_OBJECT(ImporterMilkshape)

public:

	ImporterMilkshape();

	// Creates the resource with no data.
	RESOURCE_LOADER_PREPARE(Mesh)

	// Gets the class of the resource.
	RESOURCE_LOADER_CLASS(Mesh)

	// Decodes a Milkshape3D mesh.
	bool decode(const Stream& stream, Resource* res) OVERRIDE;

	// Gets the name of this codec.
	GETTER(Name, const String, "MS3D")

	// Overrides this to return the right resource group.
	GETTER(ResourceGroup, ResourceGroup::Enum, ResourceGroup::Meshes)
};

//-----------------------------------//

NAMESPACE_RESOURCES_END

#endif