/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "ResourceProcessor.h"
#include "Engine/Resources/Mesh.h"
#include "Engine/Resources/Animation.h"
#include "Engine/Resources/Skeleton.h"

NAMESPACE_PIPELINE_BEGIN

//-----------------------------------//

/**
 * Resource processors handle the processing of resource data.
 */

API_PIPELINE REFLECT_DECLARE_CLASS(MeshProcessor)

class API_PIPELINE MeshProcessor : public ResourceProcessor
{
    REFLECT_DECLARE_OBJECT(MeshProcessor)

public:

    MeshProcessor();

    // Gets metadata about this extension.
    ExtensionMetadata* getMetadata() OVERRIDE;

    // Processes the given resource.
    bool Process(const ResourcePtr& resource);

    // Gets the processed resource type.
    Class* GetResourceType() { return MeshGetType(); }

    bool calculateBounds;
    bool calculateTangents;
};

//-----------------------------------//

NAMESPACE_PIPELINE_END
