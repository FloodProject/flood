/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Engine/API.h"
#include "Core/Containers/Array.h"
#include "Core/Utilities.h"

#include "Engine/Resources/Skeleton.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

Skeleton::Skeleton()
: bones(*AllocatorGetHeap())
{}

void Skeleton::addBone( const BonePtr& bone )
{
	bones.push_back(bone);
}

//-----------------------------------//

BonePtr Skeleton::getBone( size_t index )
{
	assert( index < bones.size() );
	return bones[index];
}

//-----------------------------------//

BonePtr Skeleton::findBone( const String& name )
{
	for( size_t i = 0; i < bones.size(); ++i )
	{
		const BonePtr& bone = bones[i];

		if( StringCompareInsensitive(bone->name, name) == 0 )
			return bone;
	}

	return nullptr;
}

//-----------------------------------//

NAMESPACE_ENGINE_END
