/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "CppSharp.h"
#include <Core/Math/Vector.h>

namespace Flood
{
    value struct Vector2;
    value struct Vector2i;
    value struct Vector3;
    value struct Vector4;
}

namespace Flood
{
    public value struct Vector3
    {
    public:

        Vector3(::Vector3* native);
        Vector3(System::IntPtr native);
        Vector3(float v);

        Vector3(float a, float b, float c);

        property float X
        {
            float get();
            void set(float);
        }

        property float Y
        {
            float get();
            void set(float);
        }

        property float Z
        {
            float get();
            void set(float);
        }

        static bool operator==(Flood::Vector3 __op, Flood::Vector3 v);

        static bool operator!=(Flood::Vector3 __op, Flood::Vector3 v);

        static bool operator<(Flood::Vector3 __op, Flood::Vector3 rhs);

        static bool operator<=(Flood::Vector3 __op, Flood::Vector3 rhs);

        static bool operator>(Flood::Vector3 __op, Flood::Vector3 rhs);

        static bool operator>=(Flood::Vector3 __op, Flood::Vector3 rhs);

        static Flood::Vector3 operator+(Flood::Vector3 __op, Flood::Vector3 v);

        static Flood::Vector3 operator-(Flood::Vector3 __op);

        static Flood::Vector3 operator-(Flood::Vector3 __op, Flood::Vector3 v);

        static Flood::Vector3 operator*(Flood::Vector3 __op, float s);

        static Flood::Vector3 operator*(Flood::Vector3 __op, Flood::Vector3 v);

        static Flood::Vector3 operator/(Flood::Vector3 __op, float s);

        void SetZero();

        float Length();

        float LengthSquared();

        float Dot(Flood::Vector3 v);

        Flood::Vector3 Cross(Flood::Vector3 v);

        float Distance(Flood::Vector3 v);

        Flood::Vector3 Project(Flood::Vector3 v);

        Flood::Vector3 Lerp(Flood::Vector3 vec, float blend);

        Flood::Vector3 Serp(Flood::Vector3 vec, float t);

        Flood::Vector3 Normalize();

        static property Flood::Vector3 Zero
        {
            Flood::Vector3 get();
        }

        static property Flood::Vector3 One
        {
            Flood::Vector3 get();
        }

        static property Flood::Vector3 UnitX
        {
            Flood::Vector3 get();
        }

        static property Flood::Vector3 UnitY
        {
            Flood::Vector3 get();
        }

        static property Flood::Vector3 UnitZ
        {
            Flood::Vector3 get();
        }

        private:
        float __X;
        float __Y;
        float __Z;
    };

    public value struct Vector4
    {
    public:

        Vector4(::Vector4* native);
        Vector4(System::IntPtr native);
        Vector4(float v);

        Vector4(float a, float b, float c, float d);

        Vector4(Flood::Vector3 v, float d);

        property float X
        {
            float get();
            void set(float);
        }

        property float Y
        {
            float get();
            void set(float);
        }

        property float Z
        {
            float get();
            void set(float);
        }

        property float W
        {
            float get();
            void set(float);
        }

        static bool operator==(Flood::Vector4 __op, Flood::Vector4 v);

        static bool operator!=(Flood::Vector4 __op, Flood::Vector4 v);

        static bool operator<(Flood::Vector4 __op, Flood::Vector4 rhs);

        static bool operator<=(Flood::Vector4 __op, Flood::Vector4 rhs);

        static bool operator>(Flood::Vector4 __op, Flood::Vector4 rhs);

        static bool operator>=(Flood::Vector4 __op, Flood::Vector4 rhs);

        static Flood::Vector4 operator+(Flood::Vector4 __op, Flood::Vector4 v);

        static Flood::Vector4 operator-(Flood::Vector4 __op);

        static Flood::Vector4 operator-(Flood::Vector4 __op, Flood::Vector4 v);

        static Flood::Vector4 operator*(Flood::Vector4 __op, float s);

        static Flood::Vector4 operator*(Flood::Vector4 __op, Flood::Vector4 v);

        static Flood::Vector4 operator/(Flood::Vector4 __op, float s);

        void SetZero();

        static property Flood::Vector4 Zero
        {
            Flood::Vector4 get();
        }

        static property Flood::Vector4 UnitX
        {
            Flood::Vector4 get();
        }

        static property Flood::Vector4 UnitY
        {
            Flood::Vector4 get();
        }

        static property Flood::Vector4 UnitZ
        {
            Flood::Vector4 get();
        }

        static property Flood::Vector4 UnitW
        {
            Flood::Vector4 get();
        }

        private:
        float __X;
        float __Y;
        float __Z;
        float __W;
    };

    public value struct Vector2
    {
    public:

        Vector2(::Vector2* native);
        Vector2(System::IntPtr native);
        Vector2(float x, float y);

        property float X
        {
            float get();
            void set(float);
        }

        property float Y
        {
            float get();
            void set(float);
        }

        static bool operator==(Flood::Vector2 __op, Flood::Vector2 v);

        static bool operator!=(Flood::Vector2 __op, Flood::Vector2 v);

        static bool operator<(Flood::Vector2 __op, Flood::Vector2 rhs);

        static bool operator<=(Flood::Vector2 __op, Flood::Vector2 rhs);

        static bool operator>(Flood::Vector2 __op, Flood::Vector2 rhs);

        static bool operator>=(Flood::Vector2 __op, Flood::Vector2 rhs);

        static Flood::Vector2 operator+(Flood::Vector2 __op, Flood::Vector2 v);

        static Flood::Vector2 operator-(Flood::Vector2 __op);

        static Flood::Vector2 operator-(Flood::Vector2 __op, Flood::Vector2 v);

        static Flood::Vector2 operator*(Flood::Vector2 __op, float s);

        static Flood::Vector2 operator/(Flood::Vector2 __op, int s);

        static operator Flood::Vector3(Flood::Vector2 __op);

        void SetZero();

        static property Flood::Vector2 Zero
        {
            Flood::Vector2 get();
        }

        static property Flood::Vector2 UnitX
        {
            Flood::Vector2 get();
        }

        static property Flood::Vector2 UnitY
        {
            Flood::Vector2 get();
        }

        private:
        float __X;
        float __Y;
    };

    public value struct Vector2i
    {
    public:

        Vector2i(::Vector2i* native);
        Vector2i(System::IntPtr native);
        Vector2i(int x, int y);

        property int X
        {
            int get();
            void set(int);
        }

        property int Y
        {
            int get();
            void set(int);
        }

        static operator Flood::Vector3(Flood::Vector2i __op);

        static bool operator==(Flood::Vector2i __op, Flood::Vector2i v);

        static bool operator!=(Flood::Vector2i __op, Flood::Vector2i v);

        static bool operator<(Flood::Vector2i __op, Flood::Vector2 rhs);

        static bool operator<=(Flood::Vector2i __op, Flood::Vector2 rhs);

        static bool operator>(Flood::Vector2i __op, Flood::Vector2 rhs);

        static bool operator>=(Flood::Vector2i __op, Flood::Vector2 rhs);

        static Flood::Vector2i operator+(Flood::Vector2i __op, Flood::Vector2i v);

        static Flood::Vector2i operator-(Flood::Vector2i __op);

        static Flood::Vector2i operator-(Flood::Vector2i __op, Flood::Vector2i v);

        static Flood::Vector2i operator*(Flood::Vector2i __op, int s);

        static Flood::Vector2i operator/(Flood::Vector2i __op, int s);

        void Zero();

        private:
        int __X;
        int __Y;
    };
}
