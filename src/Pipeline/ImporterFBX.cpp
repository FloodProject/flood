/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Pipeline/API.h"

#ifdef ENABLE_IMPORTER_FBX

// Workaround for macro conflict.
#undef Allocate

#define FBXSDK_SHARED
#include <fbxsdk.h>

#define Allocate(Type, Alloc, ...) \
	AllocateConstruct(Type, AllocateObject<Type>(Alloc), __VA_ARGS__)

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
	for(int i = 0; i < scene->GetSrcObjectCount(KFbxMesh::ClassId); i++) 
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
		bool res = converter.TriangulateInPlace(node);

		if( !res )
		{
			LogError("Error triangulating FBX mesh");
			return false;
		}

		fbxMesh = node->GetMesh();
	}
	
	// Get the mesh position vertices.
	int numControlPoints = fbxMesh->GetControlPointsCount();
	if( numControlPoints == 0 ) return false;

	KFbxVector4* positionValues = fbxMesh->GetControlPoints();
	assert( positionValues != nullptr );

	for( int i = 0; i < numControlPoints; i++ )
	{
		KFbxVector4& pos = positionValues[i];
		
		float x = (float) pos.GetAt(0);
		float y = (float) pos.GetAt(1);
		float z = (float) pos.GetAt(2);

		mesh->position.push_back(Vector3(x, y, z));
	}

	// Get the mesh texture coordinates.
	KFbxLayerElementArrayTemplate<KFbxVector2>* uvValues = nullptr;
	fbxMesh->GetTextureUV(&uvValues, KFbxLayerElement::eDIFFUSE_TEXTURES);
	KFbxLayerElement::EMappingMode uvMapping =  KFbxLayerElement::eBY_POLYGON_VERTEX;

	if(uvValues && uvValues->GetCount() > 0)
	{

	}

	KFbxGeometryElement::EMappingMode lMaterialMappingMode = KFbxGeometryElement::eNONE;
	bool hasMaterial = fbxMesh->GetElementMaterial();

	// Get the mesh indices.
	int numPolygons = fbxMesh->GetPolygonCount();

	for( int subGroup = 0; subGroup < fbxMesh->GetLayerCount(); ++subGroup )
	{
		KFbxLayer* layer = fbxMesh->GetLayer(subGroup);

		MeshGroup meshGroup;

		for( int iPolygon = 0; iPolygon < numPolygons; ++iPolygon )
		{
			int iPolygonSize = fbxMesh->GetPolygonSize(iPolygon);
			assert( iPolygonSize == 3 );

			for( int iVertex = 0; iVertex < iPolygonSize; ++iVertex )
			{
				int iControlPoint = fbxMesh->GetPolygonVertex(iPolygon, iVertex);
				meshGroup.indices.push_back(iControlPoint);
			}
		}

		mesh->groups.push_back(meshGroup);
	}

	// Mesh materials
#if 0

	if ()
	{
		lMaterialIndice = &pMesh->GetElementMaterial()->GetIndexArray();
		lMaterialMappingMode = pMesh->GetElementMaterial()->GetMappingMode();
		if (lMaterialIndice && lMaterialMappingMode == KFbxGeometryElement::eBY_POLYGON)
		{
			K_ASSERT(lMaterialIndice->GetCount() == lPolygonCount);
			if (lMaterialIndice->GetCount() == lPolygonCount)
			{
				// Count the faces of each material
				for (int lPolygonIndex = 0; lPolygonIndex < lPolygonCount; ++lPolygonIndex)
				{
					const int lMaterialIndex = lMaterialIndice->GetAt(lPolygonIndex);
					if (mSubMeshes.GetCount() < lMaterialIndex + 1)
					{
						mSubMeshes.Resize(lMaterialIndex + 1);
					}
					if (mSubMeshes[lMaterialIndex] == NULL)
					{
						mSubMeshes[lMaterialIndex] = new SubMesh;
					}
					mSubMeshes[lMaterialIndex]->TriangleCount += 1;
				}

				// Record the offset (how many vertex)
				const int lMaterialCount = mSubMeshes.GetCount();
				int lOffset = 0;
				for (int lIndex = 0; lIndex < lMaterialCount; ++lIndex)
				{
					mSubMeshes[lIndex]->IndexOffset = lOffset;
					lOffset += mSubMeshes[lIndex]->TriangleCount * 3;
					// This will be used as counter in the following procedures, reset to zero
					mSubMeshes[lIndex]->TriangleCount = 0;
				}
				K_ASSERT(lOffset == lPolygonCount * 3);
			}
		}
	}
#endif

	return true;
}

//-----------------------------------//

NAMESPACE_PIPELINE_END

#endif