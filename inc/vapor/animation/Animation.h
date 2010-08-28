/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/math/Vector3.h"
#include "vapor/math/EulerAngles.h"
#include "vapor/math/Matrix4x3.h"

FWD_DECL_INTRUSIVE(Mesh)
FWD_DECL_INTRUSIVE(Bone)
FWD_DECL_INTRUSIVE(Skeleton)

namespace vapor {

//-----------------------------------//

struct KeyFrame
{
	float time;
	Vector3 position;
	EulerAngles rotation;
};

typedef std::vector<KeyFrame> KeyFramesVector;
typedef std::map<BonePtr, KeyFramesVector> KeyFramesMap;
typedef std::pair<const BonePtr, KeyFramesVector> KeyFramesPair;

//-----------------------------------//

/**
 * An animation is composed of key frames, a collection of discrete
 * values for a given parameter, usually position and rotation, that
 * will be interpolated at a given speed rate to produce animation.
 */

class Animation : public ReferenceCounted
{
public:

	Animation();

	// Gets if the animation is looped.
	bool isLooped();

	// Gets the total animation time.
	double getTotalTime() const;

	// Gets the interpolated key frame matrix.
	Matrix4x3 getInterpolatedKeyFrameMatrix(const BonePtr& bone, double time);

	// Sets the keyframes for the animation.
	void setKeyFrames(const BonePtr& bone, const KeyFramesVector& frames);

	// Gets the key frames of the animation.
	GETTER(KeyFrames, const KeyFramesMap&, keyFrames)

protected:

	// Name of this animation.
	std::string name;

	// Loop state of this animation.
	bool looped;

	// Key frames of this animation per bone.
	KeyFramesMap keyFrames;

	// Key frames of this animation.
	KeyFramesVector keyFramesVector;
};

//-----------------------------------//

} // end namespace