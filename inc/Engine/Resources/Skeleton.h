/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Resources/Bone.h"

NAMESPACE_RESOURCES_BEGIN

//-----------------------------------//

/**
 * A skeleton is composed of bones that deform the vertices of a mesh.
 */

class Skeleton : public ReferenceCounted
{
public:

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

NAMESPACE_RESOURCES_END