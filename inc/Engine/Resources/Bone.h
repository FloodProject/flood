/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Core/Math/Vector.h"
#include "Core/Math/EulerAngles.h"
#include "Core/Math/Matrix4x3.h"
#include "Core/References.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

/**
 * Bones give structure to a skeleton.
 */

class Bone : public ReferenceCounted
{
public:

	// Name of the bone.
	String name;

	// Index of the bone.
	int32 index;

	// Index of the parent bone.
	int32 indexParent;

	// Position of the bone.
	Vector3 position;

	// Rotation of the bone.
	EulerAngles rotation;

	// Relative bone matrix.
	Matrix4x3 relativeMatrix;

	// Absolute bone matrix.
	Matrix4x3 absoluteMatrix;

	// Absolute inverse bone matrix.
	Matrix4x3 absoluteInvMatrix;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Bone );
//TYPEDEF_RESOURCE_HANDLE_FROM_TYPE( Bone );

typedef Array<BonePtr> BonesVector;

//-----------------------------------//

NAMESPACE_ENGINE_END
