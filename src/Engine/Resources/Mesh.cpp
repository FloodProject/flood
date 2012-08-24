/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Resources/API.h"
#include "Resources/Mesh.h"
#include "Resources/Skeleton.h"
#include "Resources/Animation.h"
#include "Core/Log.h"
#include "Core/Utilities.h"
#include "Math/Vector.h"
#include "Geometry/GeometryBuffer.h"

NAMESPACE_RESOURCES_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(Mesh, Resource)
REFLECT_CLASS_END()

//-----------------------------------//

Mesh::Mesh()
	: animated(false)
	, bindPose(nullptr)
	, built(false)
	, geometryBuffer(nullptr)
{
}

//-----------------------------------//

Mesh::~Mesh()
{
	LogDebug("Destroying mesh: %s", path.c_str());
	//Deallocate(geometryBuffer);
}

//-----------------------------------//

bool Mesh::isBuilt() const
{
	return built;
}

//-----------------------------------//

bool Mesh::isAnimated() const
{
	return animated;
}

//-----------------------------------//

Animation* Mesh::findAnimation( const String& name )
{
	for( size_t i = 0; i < animations.size(); i++ )
	{
		 Animation* animation = animations[i].get();

		if( StringCompareInsensitive(animation->getName(), name) == 0 )
			return animation;
	}

	return nullptr;
}

//-----------------------------------//

void Mesh::buildBounds()
{
	boundingVolume.reset();

	if( !geometryBuffer ) return;

	size_t numVertices = geometryBuffer->getNumVertices();

	for( size_t i = 0; i < groups.size(); i++ )
	{
		MeshGroup& group = groups[i];
		const std::vector<uint16>& indices = group.indices;
		
		// Update the bounding box to accomodate new geometry.
		for( size_t j = 0; j < indices.size(); j++ )
		{
			const uint16& index = indices[j];
			if( index > numVertices ) continue;

			float* v = geometryBuffer->getAttribute(VertexAttribute::Position, index);
			Vector3& vec = *(Vector3*) v;

			boundingVolume.add(vec);
		}
	}

	if( boundingVolume.isInfinite() )
		boundingVolume.setZero();
}

//-----------------------------------//

void Mesh::setupInitialVertices()
{
	if( !isAnimated() ) return;

	const BonesVector& bones = skeleton->getBones();
	size_t numVertices = geometryBuffer->getNumVertices();

	// Calculate the new vertices.
	for( size_t i = 0; i < numVertices; i++ )
	{
		int32 boneIndex = (int32) *geometryBuffer->getAttribute(VertexAttribute::BoneIndex, i);
		if( boneIndex == -1 ) continue;

		Vector3* pos = (Vector3*) geometryBuffer->getAttribute(VertexAttribute::Position, i);
		*pos = bones[boneIndex]->absoluteInvMatrix * (*pos);
	}
}

//-----------------------------------//

NAMESPACE_RESOURCES_END