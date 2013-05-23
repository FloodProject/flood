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
	array::push_back(bones, bone);
}

//-----------------------------------//

BonePtr Skeleton::getBone( size_t index )
{
	assert( index < array::size(bones) );
	return bones[index];
}

//-----------------------------------//

BonePtr Skeleton::findBone( const String& name )
{
	for( size_t i = 0; i < array::size(bones); ++i )
	{
		const BonePtr& bone = bones[i];

		if( StringCompareInsensitive(bone->name, name) == 0 )
			return bone;
	}

	return nullptr;
}

//-----------------------------------//

NAMESPACE_ENGINE_END
