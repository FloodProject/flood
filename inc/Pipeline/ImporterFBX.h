/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#ifdef ENABLE_IMPORTER_FBX

#include "Pipeline/ResourceImporter.h"
#include "Resources/Mesh.h"

#include <fbxfilesdk/fbxfilesdk_nsbegin.h>

class KFbxSdkManager;
class KFbxIOSettings;
class KFbxMesh;

#include <fbxfilesdk/fbxfilesdk_nsend.h>

NAMESPACE_PIPELINE_BEGIN

//-----------------------------------//

/**
 * Loads a mesh in the FBX format using the official SDK.
 */

REFLECT_DECLARE_CLASS(ImporterFBX)

class ImporterFBX : public ResourceImporter
{
	REFLECT_DECLARE_OBJECT(ImporterFBX)

public:

	ImporterFBX();

	// Plugin callback.
	void onPluginEnable() OVERRIDE;
	void onPluginDisable() OVERRIDE;

	// Creates the resource with no data.
	RESOURCE_LOADER_PREPARE(Mesh)

	// Gets the class of the resource.
	RESOURCE_LOADER_CLASS(Mesh)

	// Decodes a FBX scene.
	bool decode(const Stream& stream, Resource* res) OVERRIDE;

	// Converts an FBX mesh.
	bool convertMesh(FBXFILESDK_NAMESPACE::KFbxMesh*, Mesh* mesh);

	// Gets the name of this codec.
	GETTER(Name, const String, "FBX")

	// Overrides this to return the right resource group.
	GETTER(ResourceGroup, ResourceGroup::Enum, ResourceGroup::Meshes)

	float scaleFactor;

	FBXFILESDK_NAMESPACE::KFbxSdkManager* fbxSDK;
	FBXFILESDK_NAMESPACE::KFbxIOSettings* fbxIO;
};

//-----------------------------------//

NAMESPACE_PIPELINE_END

#endif