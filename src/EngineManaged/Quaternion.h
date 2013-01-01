/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <Core/Math/Quaternion.h>

namespace Flood
{
    value struct QuaternionP;
    value struct Quaternion;
    value struct EulerAngles;
    value struct Vector3;

    /// <summary>
    /// Represents a quaternion, useful for storing orientations.
    /// </summary>
    public value struct QuaternionP
    {
    };

    public value struct Quaternion
    {
        Quaternion(float u);
        Quaternion(float a, float b, float c, float d);
        Quaternion(Flood::EulerAngles angles);
        bool operator==(Flood::Quaternion q);
        bool operator!=(Flood::Quaternion q);
        void Identity();
        Flood::Quaternion Conjugate(Flood::Quaternion q);
        Flood::Quaternion operator*(Flood::Quaternion a);
        Flood::Quaternion operator*=(Flood::Quaternion a);
        float Magnitude();
        void Normalize();
        void SetToRotateAboutX(float theta);
        void SetToRotateAboutY(float theta);
        void SetToRotateAboutZ(float theta);
        void SetToRotateAboutAxis(Flood::Vector3 axis, float theta);
        float SafeAcos(float x);
        float GetRotationAngle();
        Flood::Vector3 GetRotationAxis();
        float Dot(Flood::Quaternion a);
        Flood::EulerAngles GetEulerAngles();
        Flood::Quaternion Slerp(Flood::Quaternion q, float t);
    };
}

