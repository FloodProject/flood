/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Resources/API.h"
#include "Resources/Skeleton.h"
#include "Core/Utilities.h"

NAMESPACE_RESOURCES_BEGIN

//-----------------------------------//

void Skeleton::addBone( const BonePtr& bone )
{
	bones.push_back(bone);
}

//-----------------------------------//

BonePtr Skeleton::getBone( uint index )
{
	assert( index >= 0 );
	assert( index < bones.size() );

	return bones[index];
}

//-----------------------------------//

BonePtr Skeleton::findBone( const String& name )
{
	for( uint i = 0; i < bones.size(); i++ )
	{
		const BonePtr& bone = bones[i];

		if( StringCompareInsensitive(bone->name, name) == 0 )
			return bone;
	}

	return nullptr;
}

//-----------------------------------//

NAMESPACE_RESOURCES_END