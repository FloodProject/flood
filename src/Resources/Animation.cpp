/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Resources/API.h"
#include "Resources/Animation.h"
#include "Resources/Bone.h"
#include "Math/Helpers.h"

NAMESPACE_RESOURCES_BEGIN

//-----------------------------------//

Animation::Animation()
	: looped(true)
{ }

//-----------------------------------//

void Animation::setKeyFrames(const BonePtr& bone, const KeyFramesVector& frames)
{
	keyFrames[bone] = frames;

	for( uint i = 0; i < frames.size(); i++ )
	{	
		const KeyFrame& frame = frames[i];
		keyFramesVector.push_back(frame);
	}
}

//-----------------------------------//

float Animation::getTotalTime() const
{
	if( keyFrames.empty() )
		return 0;

	float min = LimitsFloatMaximum;
	float max = 0;

	for( uint i = 0; i < keyFramesVector.size(); i++ )
	{
		const KeyFrame& keyFrame = keyFramesVector[i];

		min = std::min(min, keyFrame.time);
		max = std::max(max, keyFrame.time);
	}

	if( min == LimitsFloatMaximum )
		return 0;

	return max - min;
}

//-----------------------------------//

bool Animation::isLooped()
{
	return looped;
}

//-----------------------------------//

//#define VAPOR_ANIMATION_INTERPOLATION_LINEAR

Matrix4x3 Animation::getKeyFrameMatrix(const BonePtr& bone, float time)
{
	if( !bone || keyFrames.empty() )
		return Matrix4x3::Identity;
		 
	const KeyFramesVector& boneKeyFrames = keyFrames[bone];

	if( boneKeyFrames.empty() )
		return Matrix4x3::Identity;
	
	uint endIndex = 0;

	for( uint i = 0; i < boneKeyFrames.size(); i++ )
	{
		if( boneKeyFrames[i].time > time )
		{
			endIndex = i;
			break;
		}
	}

	Vector3 position;

#ifdef VAPOR_ANIMATION_INTERPOLATION_LINEAR
	EulerAngles rotation;
#else
	Quaternion rotation;
#endif

	if( endIndex == 0 || endIndex == boneKeyFrames.size() )
	{
		endIndex -= (endIndex != 0) ? 1 : 0;
		position = boneKeyFrames[endIndex].position;
		rotation = boneKeyFrames[endIndex].rotation;
	}
	else
	{
		int startIndex = endIndex - 1;

		const KeyFrame& keyL = boneKeyFrames[startIndex];
		const KeyFrame& keyR = boneKeyFrames[endIndex];

		float timeDelta = keyR.time - keyL.time;
		float interpolator = ((float) time - keyL.time) / timeDelta;

		position = keyL.position.lerp(keyR.position, interpolator);
		
#ifdef VAPOR_ANIMATION_INTERPOLATION_LINEAR
		rotation = (EulerAngles&)
			((Vector3&) keyL.rotation).lerp(
			(Vector3&) keyR.rotation, interpolator);
#else
		Quaternion quatL( keyL.rotation );
		Quaternion quatR( keyR.rotation );
		rotation = quatL.slerp(quatR, interpolator);
#endif
	}

	Matrix4x3 interp =
#ifdef VAPOR_ANIMATION_INTERPOLATION_LINEAR
		Matrix4x3::createRotation(rotation) *
#else
		Matrix4x3::createFromQuaternion(rotation) *
#endif
		Matrix4x3::createTranslation(position);

	return interp;
}

//-----------------------------------//

NAMESPACE_RESOURCES_END