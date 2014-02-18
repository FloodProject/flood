/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "CppSharp.h"
#include <Engine/Scene/Entity.h>

namespace Flood
{
    ref class Allocator;
    ref class Component;
    ref class Entity;
    ref class Geometry;
    ref class Transform;
}

namespace Flood
{
    /// <summary>
    /// An entity represents a game object, a container of game components. The
    /// components provide behaviour to the entity. An entity can be tagged, which
    /// can be useful to provide some different behaviour in the engine, like
    /// culling. Each entity has a Transform component by default to give it a
    /// placement in the world scene.
    /// </summary>
    public ref class Entity : ICppInstance
    {
    public:

        property ::Entity* NativePtr;
        property System::IntPtr __Instance
        {
            virtual System::IntPtr get();
            virtual void set(System::IntPtr instance);
        }

        Entity(::Entity* native);
        Entity(System::IntPtr native);
        Entity();

        Entity(System::String^ name);

        property System::String^ Name
        {
            System::String^ get();
            void set(System::String^);
        }

        property bool Visible
        {
            void set(bool);
        }

        property Flood::Entity^ Parent
        {
            Flood::Entity^ get();
            void set(Flood::Entity^);
        }

        property Flood::Transform^ Transform
        {
            Flood::Transform^ get();
        }

        property System::Collections::Generic::List<Flood::Geometry^>^ Geometry
        {
            System::Collections::Generic::List<Flood::Geometry^>^ get();
        }

    private:
        delegate void _ComponentAddedDelegate(const ::ComponentPtr& _0);
        _ComponentAddedDelegate^ _ComponentAddedDelegateInstance;
        void _ComponentAddedRaise(const ::ComponentPtr& _0);
        System::Action<Flood::Component^>^ _ComponentAdded;
    public:
        event System::Action<Flood::Component^>^ ComponentAdded
        {
            void add(System::Action<Flood::Component^>^ evt);
            void remove(System::Action<Flood::Component^>^ evt);
            void raise(Flood::Component^ _0);
        }
    private:
        delegate void _ComponentRemovedDelegate(const ::ComponentPtr& _0);
        _ComponentRemovedDelegate^ _ComponentRemovedDelegateInstance;
        void _ComponentRemovedRaise(const ::ComponentPtr& _0);
        System::Action<Flood::Component^>^ _ComponentRemoved;
    public:
        event System::Action<Flood::Component^>^ ComponentRemoved
        {
            void add(System::Action<Flood::Component^>^ evt);
            void remove(System::Action<Flood::Component^>^ evt);
            void raise(Flood::Component^ _0);
        }
        bool IsVisible();

        bool GetTag(int index);

        void SetTag(int index, bool state);

        bool AddComponent(Flood::Component^ component);

        bool RemoveComponent(Flood::Component^ component);

        Flood::Component^ GetComponent(System::String^ name);

        bool AddTransform();

        virtual void Update(float delta);

        void SendEvents();

        virtual bool Equals(System::Object^ object) override;

        virtual int GetHashCode() override;

        static Flood::Entity^ Create(Flood::Allocator^ _0);
    };
}
