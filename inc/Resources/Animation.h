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
#include "Core/References.h"

FWD_DECL_INTRUSIVE(Bone)
FWD_DECL_INTRUSIVE(Animation)

NAMESPACE_RESOURCES_BEGIN

//-----------------------------------//

struct KeyFrame
{
	// Time of the frame.
	float time;

	// Position of the frame.
	Vector3 position;

	// Rotation of the frame.
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
	float getTotalTime() const;

	// Gets the interpolated key frame matrix.
	Matrix4x3 getKeyFrameMatrix(const BonePtr& bone, float time);

	// Sets the keyframes for the animation.
	void setKeyFrames(const BonePtr& bone, const KeyFramesVector& frames);

	// Gets the key frames of the animation.
	GETTER(KeyFrames, const KeyFramesMap&, keyFrames)

	// Gets/sets the name of the animation.
	ACESSOR(Name, const String&, name)

protected:

	// Name of this animation.
	String name;

	// Loop state of this animation.
	bool looped;

	// Key frames of this animation per bone.
	KeyFramesMap keyFrames;

	// Key frames of this animation.
	KeyFramesVector keyFramesVector;
};

//-----------------------------------//

struct AnimationState
{
	// Animation data.
	AnimationPtr animation;

	// Animation time.
	float animationTime;

	// Interpolated bones matrices.
	std::vector<Matrix4x3> bonesMatrix;
};

//-----------------------------------//

struct AnimationMetadata
{
	int start;
	int end;
	String name;
	float startTime;
};

//-----------------------------------//

NAMESPACE_RESOURCES_END