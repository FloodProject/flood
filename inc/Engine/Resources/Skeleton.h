/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Engine/Resources/Bone.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

/**
 * A skeleton is composed of bones that deform the vertices of a mesh.
 */

class API_ENGINE Skeleton : public ReferenceCounted
{
public:
	Skeleton();

	// Adds a bone to the skeleton.
	void addBone( const BonePtr& bone );

	// Gets a bone from the skeleton.
	BonePtr getBone( size_t index );

	// Finds a bone from the skeleton.
	BonePtr findBone( const String& name );

	// Gets the bones of the skeleton.
	GETTER(Bones, const BonesVector&, bones)

	// Bones of the skeleton.
	BonesVector bones;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE(Skeleton)

//-----------------------------------//

NAMESPACE_ENGINE_END
