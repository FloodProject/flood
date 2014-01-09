/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#ifdef ENABLE_IMPORTER_MILKSHAPE

#include "Pipeline/ResourceImporter.h"
#include "Engine/Resources/Mesh.h"

NAMESPACE_RESOURCES_BEGIN

//-----------------------------------//

/**
 * Loads a given Milkshape3D mesh format.
 */

API_PIPELINE REFLECT_DECLARE_CLASS(ImporterMilkshape)

class API_PIPELINE ImporterMilkshape : public ResourceImporter
{
    REFLECT_DECLARE_OBJECT(ImporterMilkshape)

public:

    ImporterMilkshape();

    // Gets metadata about this extension.
    ExtensionMetadata* getMetadata() OVERRIDE;

    // Creates the resource with no data.
    RESOURCE_LOADER_PREPARE(Mesh)

    // Gets the class of the resource.
    RESOURCE_LOADER_CLASS(Mesh)

    // Decodes a Milkshape3D mesh.
    bool decode(ResourceLoadOptions& options) OVERRIDE;

    // Gets the name of this codec.
    GETTER(Name, const String, "MS3D")

    // Overrides this to return the right resource group.
    GETTER(ResourceGroup, ResourceGroup, ResourceGroup::Meshes)
};

//-----------------------------------//

NAMESPACE_RESOURCES_END

#endif