/************************************************************************
*
* Flood Project � (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Engine/API.h"
#include "Engine/Resources/Animation.h"
#include "Engine/Resources/Bone.h"
#include "Core/Math/Helpers.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

Animation::Animation()
    : looped(true)
{ }

//-----------------------------------//

void Animation::setKeyFrames(const BonePtr& bone, const KeyFramesVector& frames)
{
    keyFrames[bone] = frames;

    for( size_t i = 0; i < frames.Size(); i++ )
    {   
        const KeyFrame& frame = frames[i];
        keyFramesVector.Push(frame);
    }
}

//-----------------------------------//

float Animation::getTotalTime() const
{
    if( keyFrames.Empty() )
        return 0;

    float min = LimitsFloatMaximum;
    float max = 0;

    for( size_t i = 0; i < keyFramesVector.Size(); i++ )
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

//#define ENABLE_ANIMATION_INTERPOLATION_LINEAR

Matrix4x3 Animation::getKeyFrameMatrix(const BonePtr& bone, float time)
{
    if( !bone || keyFrames.Empty() )
        return Matrix4x3::Identity;
         
    const KeyFramesVector& boneKeyFrames = keyFrames[bone];

    if( boneKeyFrames.Empty() )
        return Matrix4x3::Identity;
    
    uint endIndex = 0;

    for( size_t i = 0; i < boneKeyFrames.Size(); i++ )
    {
        if( boneKeyFrames[i].time > time )
        {
            endIndex = i;
            break;
        }
    }

    Vector3 position;

#ifdef ENABLE_ANIMATION_INTERPOLATION_LINEAR
    EulerAngles rotation;
#else
    Quaternion rotation;
#endif

    if( endIndex == 0 || endIndex == boneKeyFrames.Size() )
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
        
#ifdef ENABLE_ANIMATION_INTERPOLATION_LINEAR
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
#ifdef ENABLE_ANIMATION_INTERPOLATION_LINEAR
        Matrix4x3::createRotation(rotation) *
#else
        Matrix4x3::createFromQuaternion(rotation) *
#endif
        Matrix4x3::createTranslation(position);

    return interp;
}

//-----------------------------------//

NAMESPACE_ENGINE_END
