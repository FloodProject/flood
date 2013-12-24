/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "CppSharp.h"
#include <Engine/Scene/Transform.h>
#include "BoundingBox.h"
#include "Component.h"
#include "Matrix4x3.h"
#include "Quaternion.h"
#include "Vector.h"

namespace Flood
{
    enum struct DebugDrawFlags : unsigned char;
    ref class Allocator;
    ref class DebugDrawer;
    ref class Transform;
}

namespace Flood
{
    /// <summary>
    /// Transform provide entities with position and orientation information. They
    /// can be used to scale, rotate and translate entities in the scene, and keep
    /// track of their bounding volumes aswell.
    /// </summary>
    public ref class Transform : Flood::Component
    {
    public:

        Transform(::Transform* native);
        Transform(System::IntPtr native);
        Transform();

        property Flood::Vector3 Position
        {
            Flood::Vector3 get();
            void set(Flood::Vector3);
        }

        property Flood::Vector3 Scale
        {
            Flood::Vector3 get();
            void set(Flood::Vector3);
        }

        property Flood::Quaternion Rotation
        {
            Flood::Quaternion get();
            void set(Flood::Quaternion);
        }

        property Flood::Matrix4x3 RotationMatrix
        {
            Flood::Matrix4x3 get();
        }

        property Flood::Matrix4x3 AbsoluteTransform
        {
            Flood::Matrix4x3 get();
            void set(Flood::Matrix4x3);
        }

        property Flood::Matrix4x3 LocalTransform
        {
            Flood::Matrix4x3 get();
        }

        property Flood::Vector3 WorldPosition
        {
            Flood::Vector3 get();
        }

        property Flood::BoundingBox BoundingVolume
        {
            Flood::BoundingBox get();
        }

        property Flood::BoundingBox WorldBoundingVolume
        {
            Flood::BoundingBox get();
        }

    private:
        delegate void _TransformedDelegate();
        _TransformedDelegate^ _TransformedDelegateInstance;
        void _TransformedRaise();
        System::Action^ _Transformed;
    public:
        event System::Action^ Transformed
        {
            void add(System::Action^ evt);
            void remove(System::Action^ evt);
            void raise();
        }
        void Reset();

        Flood::Matrix4x3 LookAt(Flood::Vector3 lookAtVector, Flood::Vector3 upVector);

        void UpdateBoundingVolume();

        void MarkBoundingVolumeDirty();

        bool RequiresBoundingVolumeUpdate();

        virtual void Update(float delta) override;

        virtual bool Equals(System::Object^ object) override;

        virtual int GetHashCode() override;

        static Flood::Transform^ Create(Flood::Allocator^ _0);
    };
}
