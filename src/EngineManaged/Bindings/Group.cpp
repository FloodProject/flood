/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Group.h"
#include "Component.h"

using namespace System;
using namespace System::Runtime::InteropServices;

Flood::Group::Group(::Group* native)
    : Flood::Entity((::Entity*)native)
{
}

Flood::Group::Group(System::IntPtr native)
    : Flood::Entity(native)
{
    auto __native = (::Group*)native.ToPointer();
}

Flood::Group::Group()
    : Flood::Entity((::Entity*)nullptr)
{
    NativePtr = new ::Group();
}

Flood::Group::Group(System::String^ name)
    : Flood::Entity((::Entity*)nullptr)
{
    auto arg0 = clix::marshalString<clix::E_UTF8>(name);
    NativePtr = new ::Group(arg0);
}

void Flood::Group::Add(Flood::Entity^ entity)
{
    auto arg0 = (::Entity*)entity->NativePtr;
    ((::Group*)NativePtr)->add(arg0);
}

bool Flood::Group::Remove(Flood::Entity^ entity)
{
    auto arg0 = (::Entity*)entity->NativePtr;
    auto __ret = ((::Group*)NativePtr)->remove(arg0);
    return __ret;
}

Flood::Entity^ Flood::Group::FindEntity(System::String^ name)
{
    auto arg0 = clix::marshalString<clix::E_UTF8>(name);
    auto __ret = ((::Group*)NativePtr)->findEntity(arg0);
    return gcnew Flood::Entity((::Entity*)__ret.get());
}

void Flood::Group::Update(float delta)
{
    ((::Group*)NativePtr)->update(delta);
}

bool Flood::Group::Equals(System::Object^ object)
{
    if (!object) return false;
    auto obj = dynamic_cast<Group^>(object);

    if (!obj) return false;
    return __Instance == obj->__Instance;
}

int Flood::Group::GetHashCode()
{
    return (int)NativePtr;
}

System::Collections::Generic::List<Flood::Entity^>^ Flood::Group::Entities::get()
{
    auto &__ret = ((::Group*)NativePtr)->getEntities();
    auto _tmp__ret = gcnew System::Collections::Generic::List<Flood::Entity^>();
    for(auto _element : __ret)
    {
        auto _marshalElement = gcnew Flood::Entity((::Entity*)_element.get());
        _tmp__ret->Add(_marshalElement);
    }
    return _tmp__ret;
}

void Flood::Group::EntityAdded::add(System::Action<Flood::Entity^>^ evt)
{
    if (!_EntityAddedDelegateInstance)
    {
        _EntityAddedDelegateInstance = gcnew _EntityAddedDelegate(this, &Flood::Group::_EntityAddedRaise);
        auto _fptr = (void (*)(const ::EntityPtr&))Marshal::GetFunctionPointerForDelegate(_EntityAddedDelegateInstance).ToPointer();
        ((::Group*)NativePtr)->onEntityAdded.Connect(_fptr);
    }
    _EntityAdded = static_cast<System::Action<Flood::Entity^>^>(System::Delegate::Combine(_EntityAdded, evt));
}

void Flood::Group::EntityAdded::remove(System::Action<Flood::Entity^>^ evt)
{
    _EntityAdded = static_cast<System::Action<Flood::Entity^>^>(System::Delegate::Remove(_EntityAdded, evt));
}

void Flood::Group::EntityAdded::raise(Flood::Entity^ _0)
{
    _EntityAdded(_0);
}

void Flood::Group::_EntityAddedRaise(const ::EntityPtr& _0)
{
    EntityAdded::raise(gcnew Flood::Entity((::Entity*)_0.get()));
}

void Flood::Group::EntityRemoved::add(System::Action<Flood::Entity^>^ evt)
{
    if (!_EntityRemovedDelegateInstance)
    {
        _EntityRemovedDelegateInstance = gcnew _EntityRemovedDelegate(this, &Flood::Group::_EntityRemovedRaise);
        auto _fptr = (void (*)(const ::EntityPtr&))Marshal::GetFunctionPointerForDelegate(_EntityRemovedDelegateInstance).ToPointer();
        ((::Group*)NativePtr)->onEntityRemoved.Connect(_fptr);
    }
    _EntityRemoved = static_cast<System::Action<Flood::Entity^>^>(System::Delegate::Combine(_EntityRemoved, evt));
}

void Flood::Group::EntityRemoved::remove(System::Action<Flood::Entity^>^ evt)
{
    _EntityRemoved = static_cast<System::Action<Flood::Entity^>^>(System::Delegate::Remove(_EntityRemoved, evt));
}

void Flood::Group::EntityRemoved::raise(Flood::Entity^ _0)
{
    _EntityRemoved(_0);
}

void Flood::Group::_EntityRemovedRaise(const ::EntityPtr& _0)
{
    EntityRemoved::raise(gcnew Flood::Entity((::Entity*)_0.get()));
}

void Flood::Group::EntityComponentAdded::add(System::Action<Flood::Component^>^ evt)
{
    if (!_EntityComponentAddedDelegateInstance)
    {
        _EntityComponentAddedDelegateInstance = gcnew _EntityComponentAddedDelegate(this, &Flood::Group::_EntityComponentAddedRaise);
        auto _fptr = (void (*)(const ::ComponentPtr&))Marshal::GetFunctionPointerForDelegate(_EntityComponentAddedDelegateInstance).ToPointer();
        ((::Group*)NativePtr)->onEntityComponentAdded.Connect(_fptr);
    }
    _EntityComponentAdded = static_cast<System::Action<Flood::Component^>^>(System::Delegate::Combine(_EntityComponentAdded, evt));
}

void Flood::Group::EntityComponentAdded::remove(System::Action<Flood::Component^>^ evt)
{
    _EntityComponentAdded = static_cast<System::Action<Flood::Component^>^>(System::Delegate::Remove(_EntityComponentAdded, evt));
}

void Flood::Group::EntityComponentAdded::raise(Flood::Component^ _0)
{
    _EntityComponentAdded(_0);
}

void Flood::Group::_EntityComponentAddedRaise(const ::ComponentPtr& _0)
{
    EntityComponentAdded::raise(gcnew Flood::Component((::Component*)_0.get()));
}

void Flood::Group::EntityComponentRemoved::add(System::Action<Flood::Component^>^ evt)
{
    if (!_EntityComponentRemovedDelegateInstance)
    {
        _EntityComponentRemovedDelegateInstance = gcnew _EntityComponentRemovedDelegate(this, &Flood::Group::_EntityComponentRemovedRaise);
        auto _fptr = (void (*)(const ::ComponentPtr&))Marshal::GetFunctionPointerForDelegate(_EntityComponentRemovedDelegateInstance).ToPointer();
        ((::Group*)NativePtr)->onEntityComponentRemoved.Connect(_fptr);
    }
    _EntityComponentRemoved = static_cast<System::Action<Flood::Component^>^>(System::Delegate::Combine(_EntityComponentRemoved, evt));
}

void Flood::Group::EntityComponentRemoved::remove(System::Action<Flood::Component^>^ evt)
{
    _EntityComponentRemoved = static_cast<System::Action<Flood::Component^>^>(System::Delegate::Remove(_EntityComponentRemoved, evt));
}

void Flood::Group::EntityComponentRemoved::raise(Flood::Component^ _0)
{
    _EntityComponentRemoved(_0);
}

void Flood::Group::_EntityComponentRemovedRaise(const ::ComponentPtr& _0)
{
    EntityComponentRemoved::raise(gcnew Flood::Component((::Component*)_0.get()));
}

void Flood::Group::EntityChanged::add(System::Action^ evt)
{
    if (!_EntityChangedDelegateInstance)
    {
        _EntityChangedDelegateInstance = gcnew _EntityChangedDelegate(this, &Flood::Group::_EntityChangedRaise);
        auto _fptr = (void (*)())Marshal::GetFunctionPointerForDelegate(_EntityChangedDelegateInstance).ToPointer();
        ((::Group*)NativePtr)->onEntityChanged.Connect(_fptr);
    }
    _EntityChanged = static_cast<System::Action^>(System::Delegate::Combine(_EntityChanged, evt));
}

void Flood::Group::EntityChanged::remove(System::Action^ evt)
{
    _EntityChanged = static_cast<System::Action^>(System::Delegate::Remove(_EntityChanged, evt));
}

void Flood::Group::EntityChanged::raise()
{
    _EntityChanged();
}

void Flood::Group::_EntityChangedRaise()
{
    EntityChanged::raise();
}

