/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Engine/API.h"

#include "Engine/Resources/Mesh.h"
#include "Engine/Resources/Skeleton.h"
#include "Engine/Resources/Animation.h"
#include "Core/Containers/Array.h"
#include "Core/Log.h"
#include "Core/Utilities.h"
#include "Core/Math/Vector.h"
#include "Graphics/GeometryBuffer.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(Mesh, Resource)
REFLECT_CLASS_END()

//-----------------------------------//

Mesh::Mesh()
	: animated(false)
	, bindPose(nullptr)
	, built(false)
	, geometryBuffer(nullptr)
	, animations(*AllocatorGetHeap())
	, groups(*AllocatorGetHeap())
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
	for( size_t i = 0; i < array::size(animations); ++i )
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

	for( size_t i = 0; i < array::size(groups); ++i )
	{
		MeshGroup& group = *groups[i];
		auto& indices = group.indices;
		
		// Update the bounding box to accomodate new geometry.
		for( size_t j = 0; j < array::size(indices); ++j )
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

NAMESPACE_ENGINE_END
