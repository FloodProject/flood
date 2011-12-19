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

#define FBXSDK_SHARED

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

class API_PIPELINE ImporterFBX : public ResourceImporter
{
	REFLECT_DECLARE_OBJECT(ImporterFBX)

public:

	ImporterFBX();
	~ImporterFBX();

	// Gets metadata about this extension.
	ExtensionMetadata* getMetadata() OVERRIDE;

	// Creates the resource with no data.
	RESOURCE_LOADER_PREPARE(Mesh)

	// Gets the class of the resource.
	RESOURCE_LOADER_CLASS(Mesh)

	// Decodes a FBX scene.
	bool decode(ResourceLoadOptions& options) OVERRIDE;

	// Converts an FBX mesh.
	bool buildMesh(FBXFILESDK_NAMESPACE::KFbxMesh*, Mesh* mesh);

	// Gets the name of this codec.
	GETTER(Name, const String, "FBX")

	// Overrides this to return the right resource group.
	GETTER(ResourceGroup, ResourceGroup::Enum, ResourceGroup::Meshes)

	float scaleFactor;

	//FBXFILESDK_NAMESPACE::KFbxSdkManager* fbxSDK;
	FBXFILESDK_NAMESPACE::KFbxIOSettings* fbxIO;
};

//-----------------------------------//

NAMESPACE_PIPELINE_END

#endif