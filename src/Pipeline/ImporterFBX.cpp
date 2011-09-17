/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Pipeline/API.h"

#ifdef ENABLE_IMPORTER_FBX

#define FBXSDK_SHARED
#include <fbxsdk.h>

#if 0
#include <kfbxsdkmanager.h>
#include <fbxfilesdk/fbxfilesdk_def.h>
#include <fbxfilesdk/kfbxio/kfbximporter.h>
#include <fbxfilesdk/kfbxio/kfbxio.h>
#include <fbxfilesdk/kfbxio/kfbxiosettings.h>
using namespace FBXFILESDK_NAMESPACE;
#endif

#include "Pipeline/ImporterFBX.h"
#include "Resources/ResourceManager.h"
#include "Resources/Skeleton.h"
#include "Core/Log.h"

NAMESPACE_PIPELINE_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(ImporterFBX, ResourceImporter)
	FIELD_PRIMITIVE(2, float, scaleFactor)
REFLECT_CLASS_END()

//-----------------------------------//

ImporterFBX::ImporterFBX()
	: fbxSDK(nullptr)
	, fbxIO(nullptr)
{
	extensions.push_back("fbx");
	extensions.push_back("dxf");
	extensions.push_back("dae");
	extensions.push_back("obj");

	LogInfo("Initializing FBX SDK version %s", FBXSDK_VERSION_STRING);

	#pragma TODO("Initialize our own FBX memory allocator")

	fbxSDK = KFbxSdkManager::Create();

	if( !fbxSDK )
	{
		LogError( "Could not initialize the FBX SDK");
		return;
	}

	fbxIO = KFbxIOSettings::Create(fbxSDK, IOSROOT);

	scaleFactor = 1.0f;
}

//-----------------------------------//

ImporterFBX::~ImporterFBX()
{
	KFBX_SAFE_DESTROY_OBJECT(fbxIO);
	KFBX_SAFE_DESTROY_OBJECT(fbxSDK);
}

//-----------------------------------//

bool ImporterFBX::decode(const Stream& stream, Resource* res)
{
	const Path& filename = stream.path;

	if( !StreamClose((Stream*) &stream) )
		return false;

	KFbxImporter* importer = KFbxImporter::Create(fbxSDK, "");

	if( !importer )
	{
		LogError("Error creating the FBX importer");
		goto cleanup;
	}
	
	// Initialize the FBX importer with default settings.
	if( !importer->Initialize(filename.c_str(), -1, fbxSDK->GetIOSettings() ))
	{
		LogError("Error initializing the FBX importer: %s", importer->GetLastErrorString());
		goto cleanup;
	}

	KFbxScene* scene = KFbxScene::Create(fbxSDK, "");

	// Import the FBX file.
	if( !importer->Import(scene) )
	{
		LogError("Error importing the FBX scene: %s", importer->GetLastErrorString());
		goto cleanup;
	}

	{
		// Convert the axis system to the OpenGL one.
		KFbxAxisSystem newAxis = KFbxAxisSystem::eOpenGL;
		newAxis.ConvertScene(scene);

		// Convert unit system to the default one.
		KFbxSystemUnit newUnits = KFbxSystemUnit( scaleFactor );
		newUnits.ConvertScene(scene);
	}

	Mesh* mesh = static_cast<Mesh*>(res);

	// Loop through all the FBX meshes and convert them to our own format.
	for (int i = 0; i < scene->GetSrcObjectCount(KFbxMesh::ClassId); i++) 
	{ 
		KFbxMesh* fbxMesh = (KFbxMesh*) scene->GetSrcObject(KFbxMesh::ClassId, i);
		convertMesh(fbxMesh, mesh);
	} 

	KFBX_SAFE_DESTROY_OBJECT(importer);
	return true;

cleanup:

	KFBX_SAFE_DESTROY_OBJECT(importer);
	return false;
}

//-----------------------------------//

bool ImporterFBX::convertMesh(KFbxMesh* fbxMesh, Mesh* mesh)
{
	KFbxNode* node = fbxMesh->GetNode();

	if(fbxMesh->GetLayerCount() == 0) return false;

	const char* meshName = fbxMesh->GetNameOnly();

	// Remove degenerate polygons.
	if( fbxMesh->CheckSamePointTwice() )
	{
		int numRemoved = fbxMesh->RemoveBadPolygons();

		if( numRemoved == -1 )
		{
			LogError("Error removing degenerate polygons of FBX mesh");
			return false;
		}
	}

	// Triangulate the mesh.
	if( !fbxMesh->IsTriangleMesh() )
	{
		KFbxGeometryConverter converter(fbxSDK);
		bool res = converter.TriangulateInPlace( fbxMesh->GetNode() );

		if( !res )
		{
			LogError("Error triangulating FBX mesh");
			return false;
		}

		fbxMesh = node->GetMesh();
	}
	
	int numControlPoints = fbxMesh->GetControlPointsCount();
	if( numControlPoints == 0 ) return false;

	KFbxVector4* positionValues = fbxMesh->GetControlPoints();

	for( int i = 0; i < numControlPoints; i++ )
	{
		KFbxVector4& pos = positionValues[i];
		
		float x = (float) pos.GetAt(0);
		float y = (float) pos.GetAt(1);
		float z = (float) pos.GetAt(2);

		mesh->position.push_back(Vector3(x, y, z));
	}

	int numPolygons = fbxMesh->GetPolygonCount();

	for( int subGroup = 0; subGroup < fbxMesh->GetLayerCount(); ++subGroup )
	{
		KFbxLayer* layer = fbxMesh->GetLayer(subGroup);

		MeshGroup meshGroup;

		for( int iPolygon = 0; iPolygon < numPolygons; ++iPolygon )
		{
			assert( fbxMesh->GetPolygonSize(iPolygon) == 3 );

			for( int iVertex = 0; iVertex < 3; ++iVertex )
			{
				int iControlPoint = fbxMesh->GetPolygonVertex(iPolygon, iVertex);
				meshGroup.indices.push_back(iControlPoint);
			}
		}

		mesh->groups.push_back(meshGroup);
	}

	return true;
}

//-----------------------------------//

NAMESPACE_PIPELINE_END

#endif