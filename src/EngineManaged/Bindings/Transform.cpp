/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Transform.h"
#include "DebugGeometry.h"
#include "Memory.h"

using namespace System;
using namespace System::Runtime::InteropServices;

Flood::Transform::Transform(::Transform* native)
    : Flood::Component((::Component*)native)
{
}

Flood::Transform::Transform(System::IntPtr native)
    : Flood::Component(native)
{
    auto __native = (::Transform*)native.ToPointer();
}

Flood::Transform::Transform()
    : Flood::Component((::Component*)nullptr)
{
    NativePtr = new ::Transform();
}

void Flood::Transform::Reset()
{
    ((::Transform*)NativePtr)->reset();
}

Flood::Matrix4x3 Flood::Transform::LookAt(Flood::Vector3 lookAtVector, Flood::Vector3 upVector)
{
    auto _marshal0 = ::Vector3();
    _marshal0.x = lookAtVector.X;
    _marshal0.y = lookAtVector.Y;
    _marshal0.z = lookAtVector.Z;
    auto arg0 = _marshal0;
    auto _marshal1 = ::Vector3();
    _marshal1.x = upVector.X;
    _marshal1.y = upVector.Y;
    _marshal1.z = upVector.Z;
    auto arg1 = _marshal1;
    auto __ret = ((::Transform*)NativePtr)->lookAt(arg0, arg1);
    return Flood::Matrix4x3((::Matrix4x3*)&__ret);
}

void Flood::Transform::UpdateBoundingVolume()
{
    ((::Transform*)NativePtr)->updateBoundingVolume();
}

void Flood::Transform::MarkBoundingVolumeDirty()
{
    ((::Transform*)NativePtr)->markBoundingVolumeDirty();
}

bool Flood::Transform::RequiresBoundingVolumeUpdate()
{
    auto __ret = ((::Transform*)NativePtr)->requiresBoundingVolumeUpdate();
    return __ret;
}

void Flood::Transform::Update(float delta)
{
    ((::Transform*)NativePtr)->update(delta);
}

bool Flood::Transform::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<Transform^>(object);

    if (!obj) return false;
    return __Instance == obj->__Instance;
}

int Flood::Transform::GetHashCode()
{
    return (int)NativePtr;
}

Flood::Transform^ Flood::Transform::Create(Flood::Allocator^ _0)
{
    auto arg0 = (::Allocator*)_0->NativePtr;
    auto __ret = ::TransformCreate(arg0);
    if (__ret == nullptr) return nullptr;
    return (__ret == nullptr) ? nullptr : gcnew Flood::Transform((::Transform*)__ret);
}

Flood::Vector3 Flood::Transform::Position::get()
{
    auto &__ret = ((::Transform*)NativePtr)->getPosition();
    return (Flood::Vector3)(Flood::Vector3((::Vector3*)&__ret));
}

void Flood::Transform::Position::set(Flood::Vector3 position)
{
    auto _marshal0 = ::Vector3();
    _marshal0.x = position.X;
    _marshal0.y = position.Y;
    _marshal0.z = position.Z;
    auto arg0 = _marshal0;
    ((::Transform*)NativePtr)->setPosition(arg0);
}

Flood::Vector3 Flood::Transform::Scale::get()
{
    auto &__ret = ((::Transform*)NativePtr)->getScale();
    return (Flood::Vector3)(Flood::Vector3((::Vector3*)&__ret));
}

void Flood::Transform::Scale::set(Flood::Vector3 scale)
{
    auto _marshal0 = ::Vector3();
    _marshal0.x = scale.X;
    _marshal0.y = scale.Y;
    _marshal0.z = scale.Z;
    auto arg0 = _marshal0;
    ((::Transform*)NativePtr)->setScale(arg0);
}

Flood::Quaternion Flood::Transform::Rotation::get()
{
    auto &__ret = ((::Transform*)NativePtr)->getRotation();
    return (Flood::Quaternion)(Flood::Quaternion((::Quaternion*)&__ret));
}

void Flood::Transform::Rotation::set(Flood::Quaternion rotation)
{
    auto _marshal0 = ::Quaternion();
    _marshal0.x = rotation.X;
    _marshal0.y = rotation.Y;
    _marshal0.z = rotation.Z;
    _marshal0.w = rotation.W;
    auto arg0 = _marshal0;
    ((::Transform*)NativePtr)->setRotation(arg0);
}

Flood::Matrix4x3 Flood::Transform::RotationMatrix::get()
{
    auto __ret = ((::Transform*)NativePtr)->getRotationMatrix();
    return Flood::Matrix4x3((::Matrix4x3*)&__ret);
}

Flood::Matrix4x3 Flood::Transform::AbsoluteTransform::get()
{
    auto &__ret = ((::Transform*)NativePtr)->getAbsoluteTransform();
    return (Flood::Matrix4x3)(Flood::Matrix4x3((::Matrix4x3*)&__ret));
}

void Flood::Transform::AbsoluteTransform::set(Flood::Matrix4x3 matrix)
{
    auto _marshal0 = ::Matrix4x3();
    _marshal0.m11 = matrix.M11;
    _marshal0.m12 = matrix.M12;
    _marshal0.m13 = matrix.M13;
    _marshal0.m21 = matrix.M21;
    _marshal0.m22 = matrix.M22;
    _marshal0.m23 = matrix.M23;
    _marshal0.m31 = matrix.M31;
    _marshal0.m32 = matrix.M32;
    _marshal0.m33 = matrix.M33;
    _marshal0.tx = matrix.Tx;
    _marshal0.ty = matrix.Ty;
    _marshal0.tz = matrix.Tz;
    auto arg0 = _marshal0;
    ((::Transform*)NativePtr)->setAbsoluteTransform(arg0);
}

Flood::Matrix4x3 Flood::Transform::LocalTransform::get()
{
    auto __ret = ((::Transform*)NativePtr)->getLocalTransform();
    return Flood::Matrix4x3((::Matrix4x3*)&__ret);
}

Flood::Vector3 Flood::Transform::WorldPosition::get()
{
    auto __ret = ((::Transform*)NativePtr)->getWorldPosition();
    return Flood::Vector3((::Vector3*)&__ret);
}

Flood::BoundingBox Flood::Transform::BoundingVolume::get()
{
    auto &__ret = ((::Transform*)NativePtr)->getBoundingVolume();
    return (Flood::BoundingBox)(Flood::BoundingBox((::BoundingBox*)&__ret));
}

Flood::BoundingBox Flood::Transform::WorldBoundingVolume::get()
{
    auto __ret = ((::Transform*)NativePtr)->getWorldBoundingVolume();
    return Flood::BoundingBox((::BoundingBox*)&__ret);
}

void Flood::Transform::Transformed::add(System::Action^ evt)
{
    if (!_TransformedDelegateInstance)
    {
        _TransformedDelegateInstance = gcnew _TransformedDelegate(this, &Flood::Transform::_TransformedRaise);
        auto _fptr = (void (*)())Marshal::GetFunctionPointerForDelegate(_TransformedDelegateInstance).ToPointer();
        ((::Transform*)NativePtr)->onTransformed.Connect(_fptr);
    }
    _Transformed = static_cast<System::Action^>(System::Delegate::Combine(_Transformed, evt));
}

void Flood::Transform::Transformed::remove(System::Action^ evt)
{
    _Transformed = static_cast<System::Action^>(System::Delegate::Remove(_Transformed, evt));
}

void Flood::Transform::Transformed::raise()
{
    _Transformed();
}

void Flood::Transform::_TransformedRaise()
{
    Transformed::raise();
}

