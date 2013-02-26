/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <Core/Math/Vector.h>

namespace Flood
{
    value struct Vector3P;
    value struct Vector3;
    value struct Vector4;
    value struct Vector2P;
    value struct Vector2;
    value struct Vector2i;

    public value struct Vector3
    {
    public:
        Vector3(::Vector3* native);
        Vector3(System::IntPtr native);
        Vector3(float v);
        Vector3(float a, float b, float c);
        bool operator==(Flood::Vector3 v);
        bool operator!=(Flood::Vector3 v);
        bool operator<(Flood::Vector3 rhs);
        bool operator>(Flood::Vector3 rhs);
        Flood::Vector3 operator+(Flood::Vector3 v);
        Flood::Vector3 operator+=(Flood::Vector3 v);
        Flood::Vector3 operator-();
        Flood::Vector3 operator-(Flood::Vector3 v);
        Flood::Vector3 operator-=(Flood::Vector3 v);
        Flood::Vector3 operator*(float s);
        Flood::Vector3 operator*(Flood::Vector3 v);
        Flood::Vector3 operator*=(float s);
        Flood::Vector3 operator/(float s);
        Flood::Vector3 operator/=(float s);
        void Zero();
        float Length();
        float LengthSquared();
        float Dot(Flood::Vector3 v);
        Flood::Vector3 Cross(Flood::Vector3 v);
        float Distance(Flood::Vector3 v);
        Flood::Vector3 Project(Flood::Vector3 v);
        Flood::Vector3 Lerp(Flood::Vector3 vec, float blend);
        Flood::Vector3 Serp(Flood::Vector3 vec, float t);
        Flood::Vector3 Normalize();
    };

    public value struct Vector4
    {
    public:
        Vector4(::Vector4* native);
        Vector4(System::IntPtr native);
        Vector4(float v);
        Vector4(float a, float b, float c, float d);
        Vector4(Flood::Vector3 v, float d);
        float X;
        float Y;
        float Z;
        float W;
        bool operator==(Flood::Vector4 v);
        bool operator!=(Flood::Vector4 v);
        bool operator<(Flood::Vector4 rhs);
        bool operator>(Flood::Vector4 rhs);
        Flood::Vector4 operator+(Flood::Vector4 v);
        Flood::Vector4 operator+=(Flood::Vector4 v);
        Flood::Vector4 operator-();
        Flood::Vector4 operator-(Flood::Vector4 v);
        Flood::Vector4 operator-=(Flood::Vector4 v);
        Flood::Vector4 operator*(float s);
        Flood::Vector4 operator*(Flood::Vector4 v);
        Flood::Vector4 operator*=(float s);
        Flood::Vector4 operator/(float s);
        Flood::Vector4 operator/=(float s);
        void Zero();
    };

    public value struct Vector2
    {
    public:
        Vector2(::Vector2* native);
        Vector2(System::IntPtr native);
        Vector2(float x, float y);
        bool operator==(Flood::Vector2 v);
        bool operator!=(Flood::Vector2 v);
        bool operator<(Flood::Vector2 rhs);
        bool operator>(Flood::Vector2 rhs);
        Flood::Vector2 operator+(Flood::Vector2 v);
        Flood::Vector2 operator+=(Flood::Vector2 v);
        Flood::Vector2 operator-();
        Flood::Vector2 operator-(Flood::Vector2 v);
        Flood::Vector2 operator-=(Flood::Vector2 v);
        Flood::Vector2 operator*(float s);
        Flood::Vector2 operator/(int s);
        void Zero();
    };

    public value struct Vector2i
    {
    public:
        Vector2i(::Vector2i* native);
        Vector2i(System::IntPtr native);
        Vector2i(int x, int y);
        int X;
        int Y;
        bool operator==(Flood::Vector2i v);
        bool operator!=(Flood::Vector2i v);
        bool operator<(Flood::Vector2i rhs);
        bool operator>(Flood::Vector2i rhs);
        Flood::Vector2i operator+(Flood::Vector2i v);
        Flood::Vector2i operator+=(Flood::Vector2i v);
        Flood::Vector2i operator-();
        Flood::Vector2i operator-(Flood::Vector2i v);
        Flood::Vector2i operator-=(Flood::Vector2i v);
        Flood::Vector2i operator*(int s);
        Flood::Vector2i operator/(int s);
        void Zero();
    };
}
