/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Pipeline/API.h"

#ifdef ENABLE_IMPORTER_FBX

#include "Pipeline/ImporterFBX.h"
#include "Resources/ResourceManager.h"
#include "Resources/Animation.h"
#include "Resources/Skeleton.h"
#include "Resources/Bone.h"
#include "Core/Log.h"

// Workaround for macro conflict.
#ifdef Allocate
#undef Allocate
#endif

#include <fbxsdk.h>

// Redefine the allocate macro.
#define Allocate(Alloc, Type, ...) \
	AllocateConstruct(Type, AllocateObject<Type>(Alloc), __VA_ARGS__)

NAMESPACE_PIPELINE_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(ImporterFBX, ResourceImporter)
	FIELD_PRIMITIVE(2, float, scaleFactor)
REFLECT_CLASS_END()

static FBXFILESDK_NAMESPACE::KFbxSdkManager* fbxSDK;

ImporterFBX::ImporterFBX()
	//: fbxSDK(nullptr)
	: fbxIO(nullptr)
	, scaleFactor(1.0f)
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
}

//-----------------------------------//

ExtensionMetadata* ImporterFBX::getMetadata()
{
	static ExtensionMetadata s_ExtensionMetadata =
	{
		"FBX",
		"Imports a FBX mesh."
	};

	return &s_ExtensionMetadata;
}

//-----------------------------------//

ImporterFBX::~ImporterFBX()
{
	KFBX_SAFE_DESTROY_OBJECT(fbxIO);
	KFBX_SAFE_DESTROY_OBJECT(fbxSDK);
}

//-----------------------------------//

static KFbxNode * FindMeshSkeletonRootBone(KFbxNode* parent);
static void BuildMeshSkeleton(KFbxSkeleton* skeleton, BonesVector& bones);

bool ImporterFBX::decode(ResourceLoadOptions& options)
{
	const Path& filename = options.stream->path;

	if( !StreamClose(options.stream) )
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
	KFbxAxisSystem newAxis = KFbxAxisSystem::OpenGL;
	newAxis.ConvertScene(scene);

	// Convert unit system to the default one.
	KFbxSystemUnit newUnits = KFbxSystemUnit( scaleFactor );
	newUnits.ConvertScene(scene);
	}

	KFbxNode* root = scene->GetRootNode();
	int childCount = root->GetChildCount();

	KFbxMesh* fbxMesh = nullptr;
	KFbxSkeleton* fbxSkeleton = nullptr;

	// Search for the nodes we need.
	for( int childIndex = 0; childIndex < childCount; ++childIndex )
	{
		KFbxNode* childNode = root->GetChild( childIndex );

		KFbxNodeAttribute* nodeAttribute = childNode->GetNodeAttribute();
		if( !nodeAttribute ) continue;

		LogDebug("Found node '%s' / '%s'", childNode->GetName(), nodeAttribute->GetClassId().GetName());

		KFbxNodeAttribute::EAttributeType attributeType = nodeAttribute->GetAttributeType();
		
		switch(attributeType)
		{
		case KFbxNodeAttribute::eMESH:
			fbxMesh = childNode->GetMesh();
			break;
		case KFbxNodeAttribute::eSKELETON:
			fbxSkeleton = childNode->GetSkeleton();
			break;
		};

		if(fbxMesh && fbxSkeleton) break;
	}

	if( !fbxSkeleton )
	{
		// If the mesh skeleton was not found, try another method.
		KFbxNode* skeletonNode = FindMeshSkeletonRootBone(root);
		if( skeletonNode ) fbxSkeleton = skeletonNode->GetSkeleton();
	}

	Mesh* mesh = static_cast<Mesh*>(options.resource);
	buildMesh(fbxMesh, mesh);

	// Get the mesh skeleton.
	bool hasAnimation = mesh->getSkeleton() != nullptr;

	if(hasAnimation)
	{
		Skeleton* skeleton = AllocateThis(Skeleton);
		BuildMeshSkeleton(fbxSkeleton, skeleton->bones);

		mesh->skeleton = skeleton;
	}

	KFBX_SAFE_DESTROY_OBJECT(importer);
	return true;

cleanup:

	KFBX_SAFE_DESTROY_OBJECT(importer);
	return false;
}

//-----------------------------------//

static bool GetValueFromLayer(KFbxLayerElementUV* le, int Index, KFbxVector2& TexCoord)
{
	switch (le->GetMappingMode())
	{
	case KFbxLayerElement::eBY_CONTROL_POINT:
	{
		switch (le->GetReferenceMode())
		{
		case KFbxLayerElement::eDIRECT:
		{
			TexCoord = le->GetDirectArray().GetAt(Index);
			break;
		}
		case KFbxLayerElement::eINDEX_TO_DIRECT:
		{
			int id = le->GetIndexArray().GetAt(Index);
			TexCoord = le->GetDirectArray().GetAt(id);
			break;
		}
		default:
			LogWarn("Unknown FBX layer mapping mode");
			return false;
		}
		break;
	}
	case KFbxLayerElement::eBY_POLYGON_VERTEX:
	{
		switch (le->GetReferenceMode())
		{
			case KFbxLayerElement::eDIRECT:
			case KFbxLayerElement::eINDEX_TO_DIRECT:
			{
				TexCoord = le->GetDirectArray().GetAt(Index);
				break;
			}
			default:
				LogWarn("Unknown FBX layer mapping mode");
				return false;
		}
	} }

	return true;
}

//-----------------------------------//

static inline Vector2 ConvertFromFBX(const KFbxVector2& vec)
{
	float x = (float) vec.GetAt(0);
	float y = (float) vec.GetAt(1);

	return Vector2(x, y);
}

static inline Vector3 ConvertFromFBX(const KFbxVector4& vec)
{
	float x = (float) vec.GetAt(0);
	float y = (float) vec.GetAt(1);
	float z = (float) vec.GetAt(2);

	return Vector3(x, y, z);
}

//-----------------------------------//

static bool CleanMesh(KFbxMesh*& mesh)
{
	KFbxNode* node = mesh->GetNode();

	// Remove degenerate polygons.
	if( mesh->CheckSamePointTwice() )
	{
		int numRemoved = mesh->RemoveBadPolygons();

		if( numRemoved == -1 )
		{
			LogError("Error removing degenerate polygons of FBX mesh");
			return false;
		}
	}

	// Triangulate the mesh.
	if( !mesh->IsTriangleMesh() )
	{
		KFbxGeometryConverter converter(fbxSDK);
		bool res = converter.TriangulateInPlace(node);

		if( !res )
		{
			LogError("Error triangulating FBX mesh");
			return false;
		}

		mesh = node->GetMesh();
	}

	return true;
}

//-----------------------------------//

static void BuildMeshSkinning(KFbxMesh* fbxMesh, std::vector<int>& boneIndices)
{
	// Get the mesh skinning.
	int numSkins = fbxMesh->GetDeformerCount(KFbxDeformer::eSKIN);

	if(numSkins > 0)
	{
		int numControlPoints = fbxMesh->GetControlPointsCount();
		boneIndices.resize(numControlPoints, -1);
	}
	
	for(int iSkin = 0; iSkin < numSkins; ++iSkin)
	{
		KFbxSkin* skin = (KFbxSkin*) fbxMesh->GetDeformer(iSkin, KFbxDeformer::eSKIN);

		int clusterCount = skin->GetClusterCount();
		if( clusterCount == 0 ) continue;

		for (int iCluster = 0; iCluster < clusterCount; ++iCluster)
		{
			KFbxCluster* cluster = skin->GetCluster(iCluster);
			KFbxNode* bone = cluster->GetLink();

			if( !bone )
			{
				LogWarn("Cluster %d has no link", iCluster);
				continue;
			}

			int numInfluencedVertices = cluster->GetControlPointIndicesCount();

			int* pIndexArray = cluster->GetControlPointIndices();
			double *pWeightArray = cluster->GetControlPointWeights();

			LogDebug("Cluster %02d '%s' influences %d vertices", iCluster,
				bone->GetName(), numInfluencedVertices);

			for(int iControlPoint = 0; iControlPoint < numInfluencedVertices; ++iControlPoint)
			{
				int iControlPointIndex = pIndexArray[iControlPoint];

				int boneIndex = boneIndices[iControlPointIndex];

				if( boneIndex != -1 )
					LogDebug("Control point %d is already mapped to bone %d", iControlPointIndex, boneIndex);

				boneIndices[iControlPointIndex] = iCluster;
			}

			#pragma TODO("Support FBX skeleton bone weighting")
		}
	}
}

//-----------------------------------//

static void BuildMeshSkeletonRecursive(const KFbxNode* boneNode, int parentBoneIndex,
	BonesVector& bones)
{
	Bone* bone = AllocateHeap(Bone);
	bone->name = boneNode->GetName();
	bone->index = bones.size();
	bone->indexParent = parentBoneIndex;
	bones.push_back(bone);

	LogDebug("Found bone: '%s' parent: %d", bone->name.c_str(), bone->indexParent);

	int childCount = boneNode->GetChildCount();

	for( int childIndex = 0; childIndex < childCount; ++childIndex )
	{
		const KFbxNode* childNode = boneNode->GetChild( childIndex );
		BuildMeshSkeletonRecursive( childNode, bone->index, bones );
	}
}

static void BuildMeshSkeleton(KFbxSkeleton* skeleton, BonesVector& bones)
{
	if( !skeleton ) return;
	KFbxNode* boneNode = skeleton->GetNode();
	BuildMeshSkeletonRecursive(boneNode, -1, bones);
}

//-----------------------------------//

static bool IsMeshSkeleton(KFbxNode * node)
{
	KFbxNodeAttribute * attrib = node->GetNodeAttribute();
	if( !attrib ) return false;

	return attrib->GetAttributeType() == KFbxNodeAttribute::eSKELETON
		&& ((KFbxSkeleton *) attrib)->GetSkeletonType() == KFbxSkeleton::eLIMB_NODE;
}

static KFbxNode * FindMeshSkeletonRootBone(KFbxNode* parent)
{
	KFbxNode * parentParent = parent->GetParent();
	
	if (IsMeshSkeleton(parent) && parentParent && !IsMeshSkeleton(parentParent))
		return parent;
	
	for (int i = 0; i < parent->GetChildCount(); ++i)
	{
		KFbxNode * result = FindMeshSkeletonRootBone(parent->GetChild(i));
		if (result != nullptr) return result;
	}

	return nullptr;
}

//-----------------------------------//

static KFbxMatrix CalculateNodeTransformMatrix(KFbxNode* node)
{
	// Build a matrix with the geometric transformations.
	const KFbxVector4 vecT = node->GetGeometricTranslation(KFbxNode::eSOURCE_SET);
	const KFbxVector4 vecR = node->GetGeometricRotation(KFbxNode::eSOURCE_SET);
	const KFbxVector4 vecS = node->GetGeometricScaling(KFbxNode::eSOURCE_SET);
	const KFbxMatrix matGeom(vecT, vecR, vecS);

	// Transform the global matrix by the geometric matrix.
	KFbxMatrix matGlobal = node->EvaluateGlobalTransform();
	matGlobal *= matGeom;

	return matGlobal;
}

//-----------------------------------//

bool ImporterFBX::buildMesh(KFbxMesh* fbxMesh, Mesh* mesh)
{
	if( !fbxMesh ) return false;
	
	if(fbxMesh->GetLayerCount() == 0) return false;

	// Clean up the FBX mesh.
	if( !CleanMesh(fbxMesh) ) return false;

	KFbxNode* node = fbxMesh->GetNode();
	const char* meshName = fbxMesh->GetNameOnly();

	// Get the mesh position vertices.
	int numControlPoints = fbxMesh->GetControlPointsCount();
	if( numControlPoints == 0 ) return false;

	KFbxVector4* positionValues = fbxMesh->GetControlPoints();
	assert( positionValues != nullptr );

	// Get the sets of UV.
	uint8 uvSetsCount = fbxMesh->GetElementUVCount();
	bool hasUV = uvSetsCount > 0;

    KStringList uvSetsNames;
    fbxMesh->GetUVSetNames(uvSetsNames);

	// Material setup.
	KFbxGeometryElement::EMappingMode matMapping = KFbxGeometryElement::eNONE;
	bool hasMaterial = fbxMesh->GetElementMaterial() != 0;

	// Get the mesh skinning.
	std::vector<int> boneIndices;
	BuildMeshSkinning(fbxMesh, boneIndices);
	bool isSkinned = !boneIndices.empty();

	// Transform setup.
	KFbxMatrix nodeTransform = CalculateNodeTransformMatrix(node);

	// Build a new geometry buffer and setup its vertex format.
	GeometryBuffer* gb = AllocateThis(GeometryBuffer);
	
	assert( !mesh->getGeometryBuffer() );
	mesh->setGeometryBuffer(gb);

	// Setup the vertex declarations.
	VertexDeclaration& vertexDecls = gb->declarations;

	VertexElementP pos = { VertexAttribute::Position, VertexDataType::Float, 3 };
	vertexDecls.add(pos);

	if( /*isSkinned*/true )
	{
		VertexElementP bone = { VertexAttribute::BoneIndex, VertexDataType::Float, 1 };
		vertexDecls.add(bone);
	}

	for( uint8 i = 0; i < uvSetsCount; ++i )
	{
		VertexAttribute::Enum attribute = (VertexAttribute::Enum)((int)VertexAttribute::TexCoord0 + i);
		VertexElementP uv = { attribute, VertexDataType::Float, 2 };
		vertexDecls.add(uv);
	}

	vertexDecls.calculateStrides();

	const size_t vertexSize = vertexDecls.getVertexSize();
	size_t vertexCount = 0;

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
				//meshGroup.indices.push_back(iControlPoint);

				KFbxVector4 control = positionValues[iControlPoint];
				control = nodeTransform.MultNormalize(control);

				struct FbxVertex
				{
					Vector3P position;
					float boneIndex;
					Vector2 uv0;
					Vector2 uv1;
				};

				assert( vertexSize <= sizeof(FbxVertex) );

				// We are not using indices yet, so we create a new vertex.

				FbxVertex vertex;
				vertex.position = ConvertFromFBX(control);

				if(isSkinned)
					vertex.boneIndex = boneIndices[iControlPoint];

				uint16 uvOffset = offsetof(FbxVertex, uv0);
				uint8 uvSize = sizeof(Vector2);

				if(hasUV)
				{
					uint8* uvStart = (uint8*) &vertex + uvOffset;

					for(uint8 uvSet = 0; uvSet < uvSetsCount; ++uvSet)
					{
						KFbxVector2 tex;

						const char* uv = uvSetsNames[uvSet].Buffer();
						fbxMesh->GetPolygonVertexUV(iPolygon, iVertex, uv, tex);

						// Invert the Y tex coord to respect the OpenGL axis.
						tex.SetAt(1, 1 - tex.GetAt(1));

						// Calculate the UV offset in the vertex.
						Vector2* pUV = (Vector2*) (uvStart + uvSet * uvSize);
						*pUV = ConvertFromFBX(tex);
					}
				}

				gb->add((uint8*) &vertex, vertexSize);

				meshGroup.indices.push_back(vertexCount++);
			}
		}

#if 0
		KFbxLayerElementMaterial* mat = layer->GetMaterials();
		bool hasMaterial = mat != nullptr;

		if( !hasMaterial )
		{
			LogWarn("FBX layer '%d' does not have material", subGroup);
			goto out;
		}
#endif

		KFbxNode* node = fbxMesh->GetNode();
		int numMaterials = node->GetSrcObjectCount(KFbxSurfaceMaterial::ClassId);

		for (int iMaterial = 0; iMaterial < numMaterials; iMaterial++)
		{
			if( !meshGroup.material.texture.empty() )
				continue;

			KFbxSurfaceMaterial* material = KFbxCast<KFbxSurfaceMaterial>(
				node->GetSrcObject(KFbxSurfaceMaterial::ClassId, iMaterial));

			if( !material ) continue;

			KFbxProperty diffuseProp = material->FindProperty(KFbxSurfaceMaterial::sDiffuse);
			if( !diffuseProp.IsValid() ) continue;

			int numTextures = diffuseProp.GetSrcObjectCount(KFbxFileTexture::ClassId);
			
			for (int iTexture = 0; iTexture < numTextures; iTexture++)
			{
				KFbxFileTexture* texture = KFbxCast<KFbxFileTexture>(
					diffuseProp.GetSrcObject(KFbxTexture::ClassId, iTexture)); 

				if( !texture ) continue;

				const char* fileName = texture->GetFileName();
				if( !fileName || (strlen(fileName) == 0) ) continue;

				meshGroup.material.alpha = texture->GetAlphaSource() != KFbxTexture::eNONE;
				meshGroup.material.name = material->GetName();
				meshGroup.material.texture = PathGetFile(fileName);
				break;
			}
		}

		if( meshGroup.material.texture.empty() )
			LogWarn("No material texture found for FBX mesh");

		mesh->groups.push_back(meshGroup);
	}

	return true;
}

//-----------------------------------//

NAMESPACE_PIPELINE_END

#endif