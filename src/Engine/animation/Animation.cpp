/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/animation/Animation.h"
#include "vapor/animation/Bone.h"
#include "vapor/math/Math.h"

namespace vapor {

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

double Animation::getTotalTime() const
{
	if( keyFrames.empty() )
		return 0;

	float min = Limits::FloatMaximum;
	float max = 0;

	for( uint i = 0; i < keyFramesVector.size(); i++ )
	{
		const KeyFrame& keyFrame = keyFramesVector[i];

		min = std::min(min, keyFrame.time);
		max = std::max(max, keyFrame.time);
	}

	if( min == Limits::FloatMaximum )
		return 0;

	return max - min;
}

//-----------------------------------//

bool Animation::isLooped()
{
	return looped;
}

//-----------------------------------//

Matrix4x3 Animation::getKeyFrameMatrix(const BonePtr& bone, double time)
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
	//EulerAngles rotation;
	Quaternion rotation;

	if( endIndex == 0 || endIndex == boneKeyFrames.size() )
	{
		endIndex -= (endIndex != 0) ? 1 : 0;
		position = boneKeyFrames[endIndex].position;
		rotation = boneKeyFrames[endIndex].rotation;
	}
	else
	{
		int startIndex = endIndex - 1;

		const KeyFrame& keyLeft = boneKeyFrames[startIndex];
		const KeyFrame& keyRight = boneKeyFrames[endIndex];

		float timeDelta = keyRight.time - keyLeft.time;
		float interpolator = ((float) time - keyLeft.time) / timeDelta;

		position = keyLeft.position.lerp(keyRight.position, interpolator);
		
		//Vector3 rot_left = (Vector3&) left.rotation;
		//Vector3 rot_right = (Vector3&) right.rotation;
		//Vector3 rot = rot_left.lerp(rot_right, interpolator);
		//rotation = (EulerAngles&) rot;
		
		Quaternion rotLeft( keyLeft.rotation );
		Quaternion rotRight( keyRight.rotation );
		rotation = rotLeft.slerp(rotRight, interpolator);
	}

	Matrix4x3 interp = 
		//Matrix4x3::createRotation(rotation) *
		Matrix4x3::createFromQuaternion(rotation) *
		Matrix4x3::createTranslation(position);

	return interp;
}

//-----------------------------------//

} // end namespace