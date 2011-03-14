/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Math/Vector.h"
#include "Math/EulerAngles.h"
#include "Math/Matrix4x3.h"
#include "ReferenceCount.h"

namespace vapor {

//-----------------------------------//

/**
 * Bones give structure to a skeleton.
 */

class Bone : public ReferenceCounted
{
public:

	// Name of the bone.
	std::string name;

	// Index of the bone.
	int index;

	// Index of the parent bone.
	int indexParent;

	// Position of the bone.
	Vector3 position;

	// Rotation of the bone.
	EulerAngles rotation;

	// Relative bone matrix.
	Matrix4x3 relativeMatrix;

	// Absolute bone matrix.
	Matrix4x3 absoluteMatrix;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE(Bone)
typedef std::vector<BonePtr> BonesVector;

//-----------------------------------//

} // end namespace