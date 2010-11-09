/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/animation/Skeleton.h"
#include "vapor/Utilities.h"

namespace vapor {

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

BonePtr Skeleton::findBone( const std::string& name )
{
	foreach( const BonePtr& bone, bones )
	{
		if( String::compareInsensitive(bone->name, name) == 0 )
			return bone;
	}

	return nullptr;
}

//-----------------------------------//

} // end namespace