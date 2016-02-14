/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Entity.h"
#include "Component.h"
#include "Geometry.h"
#include "Memory.h"
#include "Transform.h"

using namespace System;
using namespace System::Runtime::InteropServices;

Flood::Entity::Entity(::Entity* native)
{
    NativePtr = native;
}

Flood::Entity::Entity(System::IntPtr native)
{
    auto __native = (::Entity*)native.ToPointer();
    NativePtr = __native;
}

Flood::Entity::Entity()
{
    NativePtr = new ::Entity();
}

Flood::Entity::Entity(System::String^ name)
{
    auto arg0 = clix::marshalString<clix::E_UTF8>(name);
    NativePtr = new ::Entity(arg0);
}

bool Flood::Entity::IsVisible()
{
    auto __ret = ((::Entity*)NativePtr)->isVisible();
    return __ret;
}

bool Flood::Entity::GetTag(int index)
{
    auto arg0 = (::int32)(::int32_t)index;
    auto __ret = ((::Entity*)NativePtr)->getTag(arg0);
    return __ret;
}

void Flood::Entity::SetTag(int index, bool state)
{
    auto arg0 = (::int32)(::int32_t)index;
    ((::Entity*)NativePtr)->setTag(arg0, state);
}

bool Flood::Entity::AddComponent(Flood::Component^ component)
{
    auto arg0 = (::Component*)component->NativePtr;
    auto __ret = ((::Entity*)NativePtr)->addComponent(arg0);
    return __ret;
}

bool Flood::Entity::RemoveComponent(Flood::Component^ component)
{
    auto arg0 = (::Component*)component->NativePtr;
    auto __ret = ((::Entity*)NativePtr)->removeComponent(arg0);
    return __ret;
}

Flood::Component^ Flood::Entity::GetComponent(System::String^ name)
{
    auto _arg0 = clix::marshalString<clix::E_UTF8>(name);
    auto arg0 = _arg0.c_str();
    auto __ret = ((::Entity*)NativePtr)->getComponent(arg0);
    return (__ret.get() == nullptr) ? nullptr : gcnew Flood::Component((::Component*)__ret.get());
}

bool Flood::Entity::AddTransform()
{
    auto __ret = ((::Entity*)NativePtr)->addTransform();
    return __ret;
}

void Flood::Entity::Update(float delta)
{
    ((::Entity*)NativePtr)->update(delta);
}

void Flood::Entity::SendEvents()
{
    ((::Entity*)NativePtr)->sendEvents();
}

bool Flood::Entity::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<Entity^>(object);

    if (!obj) return false;
    return __Instance == obj->__Instance;
}

int Flood::Entity::GetHashCode()
{
    return (int)NativePtr;
}

Flood::Entity^ Flood::Entity::Create(Flood::Allocator^ _0)
{
    auto arg0 = (::Allocator*)_0->NativePtr;
    auto __ret = ::EntityCreate(arg0);
    if (__ret == nullptr) return nullptr;
    return (__ret == nullptr) ? nullptr : gcnew Flood::Entity((::Entity*)__ret);
}

System::IntPtr Flood::Entity::__Instance::get()
{
    return System::IntPtr(NativePtr);
}

void Flood::Entity::__Instance::set(System::IntPtr object)
{
    NativePtr = (::Entity*)object.ToPointer();
}

System::String^ Flood::Entity::Name::get()
{
    auto &__ret = ((::Entity*)NativePtr)->getName();
    return (System::String^)(clix::marshalString<clix::E_UTF8>(__ret));
}

void Flood::Entity::Name::set(System::String^ name)
{
    auto arg0 = clix::marshalString<clix::E_UTF8>(name);
    ((::Entity*)NativePtr)->setName(arg0);
}

void Flood::Entity::Visible::set(bool v)
{
    ((::Entity*)NativePtr)->setVisible(v);
}

Flood::Entity^ Flood::Entity::Parent::get()
{
    auto __ret = ((::Entity*)NativePtr)->getParent();
    if (__ret == nullptr) return nullptr;
    return (__ret == nullptr) ? nullptr : gcnew Flood::Entity((::Entity*)__ret);
}

void Flood::Entity::Parent::set(Flood::Entity^ v)
{
    auto arg0 = (::Entity*)v->NativePtr;
    ((::Entity*)NativePtr)->setParent(arg0);
}

Flood::Transform^ Flood::Entity::Transform::get()
{
    auto __ret = ((::Entity*)NativePtr)->getTransform();
    return (__ret.get() == nullptr) ? nullptr : gcnew Flood::Transform((::Transform*)__ret.get());
}

System::Collections::Generic::List<Flood::Geometry^>^ Flood::Entity::Geometry::get()
{
    auto __ret = ((::Entity*)NativePtr)->getGeometry();
    auto _tmp__ret = gcnew System::Collections::Generic::List<Flood::Geometry^>();
    for(auto _element : __ret)
    {
        auto _marshalElement = (_element.get() == nullptr) ? nullptr : gcnew Flood::Geometry((::Geometry*)_element.get());
        _tmp__ret->Add(_marshalElement);
    }
    return _tmp__ret;
}

void Flood::Entity::ComponentAdded::add(System::Action<Flood::Component^>^ evt)
{
    if (!_ComponentAddedDelegateInstance)
    {
        _ComponentAddedDelegateInstance = gcnew _ComponentAddedDelegate(this, &Flood::Entity::_ComponentAddedRaise);
        auto _fptr = (void (*)(const ::ComponentPtr&))Marshal::GetFunctionPointerForDelegate(_ComponentAddedDelegateInstance).ToPointer();
        ((::Entity*)NativePtr)->onComponentAdded.Connect(_fptr);
    }
    _ComponentAdded = static_cast<System::Action<Flood::Component^>^>(System::Delegate::Combine(_ComponentAdded, evt));
}

void Flood::Entity::ComponentAdded::remove(System::Action<Flood::Component^>^ evt)
{
    _ComponentAdded = static_cast<System::Action<Flood::Component^>^>(System::Delegate::Remove(_ComponentAdded, evt));
}

void Flood::Entity::ComponentAdded::raise(Flood::Component^ _0)
{
    _ComponentAdded(_0);
}

void Flood::Entity::_ComponentAddedRaise(const ::ComponentPtr& _0)
{
    ComponentAdded::raise((_0.get() == nullptr) ? nullptr : gcnew Flood::Component((::Component*)_0.get()));
}

void Flood::Entity::ComponentRemoved::add(System::Action<Flood::Component^>^ evt)
{
    if (!_ComponentRemovedDelegateInstance)
    {
        _ComponentRemovedDelegateInstance = gcnew _ComponentRemovedDelegate(this, &Flood::Entity::_ComponentRemovedRaise);
        auto _fptr = (void (*)(const ::ComponentPtr&))Marshal::GetFunctionPointerForDelegate(_ComponentRemovedDelegateInstance).ToPointer();
        ((::Entity*)NativePtr)->onComponentRemoved.Connect(_fptr);
    }
    _ComponentRemoved = static_cast<System::Action<Flood::Component^>^>(System::Delegate::Combine(_ComponentRemoved, evt));
}

void Flood::Entity::ComponentRemoved::remove(System::Action<Flood::Component^>^ evt)
{
    _ComponentRemoved = static_cast<System::Action<Flood::Component^>^>(System::Delegate::Remove(_ComponentRemoved, evt));
}

void Flood::Entity::ComponentRemoved::raise(Flood::Component^ _0)
{
    _ComponentRemoved(_0);
}

void Flood::Entity::_ComponentRemovedRaise(const ::ComponentPtr& _0)
{
    ComponentRemoved::raise((_0.get() == nullptr) ? nullptr : gcnew Flood::Component((::Component*)_0.get()));
}

