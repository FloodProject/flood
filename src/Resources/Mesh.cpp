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
#include "Math/Vector3.h"
#include "Utilities.h"

namespace vapor {

//-----------------------------------//

BEGIN_CLASS_PARENT_ABSTRACT(Mesh, Resource)
END_CLASS()

//-----------------------------------//

Mesh::Mesh()
	: animated(false)
	, bindPose(nullptr)
	, built(false)
{ }

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

AnimationPtr Mesh::findAnimation( const std::string& name )
{
	for( uint i = 0; i < animations.size(); i++ )
	{
		 const AnimationPtr& animation = animations[i];

		if( String::compareInsensitive(animation->getName(), name) == 0 )
			return animation;
	}

	return nullptr;
}

//-----------------------------------//

void Mesh::buildBounds()
{
	boundingVolume.reset();

	for( uint i = 0; i < groups.size(); i++ )
	{
		MeshGroup& group = groups[i];

		// Update the bounding box to accomodate new geometry.
		for( uint j = 0; j < group.position.size(); j++ )
		{
			const Vector3& v = group.position[j];
			boundingVolume.add( v );
		}
	}

	if( boundingVolume.isInfinite() )
		boundingVolume.setZero();
}

//-----------------------------------//

void Mesh::setupInitialVertices()
{
	if( !isAnimated() )
		return;

	if( !skeleton )
		return;

	for( uint i = 0; i < groups.size(); i++ )
	{
		MeshGroup& group = groups[i];

		for( uint j = 0; j < group.position.size(); j++ )
		{
			int boneIndex = (int) group.bones[j];

			if( boneIndex == -1 )
				continue;
		
			const BonePtr& bone = skeleton->getBone(boneIndex);
		
			Matrix4x3 invJoint = bone->absoluteMatrix.inverse();
		
			Vector3& vertex = group.position[j];			
			vertex = invJoint*vertex;
		}
	}
}

//-----------------------------------//

} // end namespace