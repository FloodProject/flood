/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "CppSharp.h"
#include <Engine/Scene/Group.h>
#include "Entity.h"
#include "StringConverter.h"

namespace Flood
{
    ref class Component;
    ref class Group;
}

namespace Flood
{
    /// <summary>
    /// Groups are nodes that have the special property of being able to be
    /// attached to each other. If you attach a node to another node, they will be
    /// in a hierarchical relationship and the parent node will influence the child
    /// node transformations.
    /// </summary>
    public ref class Group : Flood::Entity
    {
    public:

        Group(::Group* native);
        Group(System::IntPtr native);
        Group();

        Group(System::String^ name);

        property System::Collections::Generic::List<Flood::Entity^>^ Entities
        {
            System::Collections::Generic::List<Flood::Entity^>^ get();
        }

    private:
        delegate void _EntityAddedDelegate(const ::EntityPtr& _0);
        _EntityAddedDelegate^ _EntityAddedDelegateInstance;
        void _EntityAddedRaise(const ::EntityPtr& _0);
        System::Action<Flood::Entity^>^ _EntityAdded;
    public:
        event System::Action<Flood::Entity^>^ EntityAdded
        {
            void add(System::Action<Flood::Entity^>^ evt);
            void remove(System::Action<Flood::Entity^>^ evt);
            void raise(Flood::Entity^ _0);
        }
    private:
        delegate void _EntityRemovedDelegate(const ::EntityPtr& _0);
        _EntityRemovedDelegate^ _EntityRemovedDelegateInstance;
        void _EntityRemovedRaise(const ::EntityPtr& _0);
        System::Action<Flood::Entity^>^ _EntityRemoved;
    public:
        event System::Action<Flood::Entity^>^ EntityRemoved
        {
            void add(System::Action<Flood::Entity^>^ evt);
            void remove(System::Action<Flood::Entity^>^ evt);
            void raise(Flood::Entity^ _0);
        }
    private:
        delegate void _EntityComponentAddedDelegate(const ::ComponentPtr& _0);
        _EntityComponentAddedDelegate^ _EntityComponentAddedDelegateInstance;
        void _EntityComponentAddedRaise(const ::ComponentPtr& _0);
        System::Action<Flood::Component^>^ _EntityComponentAdded;
    public:
        event System::Action<Flood::Component^>^ EntityComponentAdded
        {
            void add(System::Action<Flood::Component^>^ evt);
            void remove(System::Action<Flood::Component^>^ evt);
            void raise(Flood::Component^ _0);
        }
    private:
        delegate void _EntityComponentRemovedDelegate(const ::ComponentPtr& _0);
        _EntityComponentRemovedDelegate^ _EntityComponentRemovedDelegateInstance;
        void _EntityComponentRemovedRaise(const ::ComponentPtr& _0);
        System::Action<Flood::Component^>^ _EntityComponentRemoved;
    public:
        event System::Action<Flood::Component^>^ EntityComponentRemoved
        {
            void add(System::Action<Flood::Component^>^ evt);
            void remove(System::Action<Flood::Component^>^ evt);
            void raise(Flood::Component^ _0);
        }
    private:
        delegate void _EntityChangedDelegate();
        _EntityChangedDelegate^ _EntityChangedDelegateInstance;
        void _EntityChangedRaise();
        System::Action^ _EntityChanged;
    public:
        event System::Action^ EntityChanged
        {
            void add(System::Action^ evt);
            void remove(System::Action^ evt);
            void raise();
        }
        void Add(Flood::Entity^ entity);

        bool Remove(Flood::Entity^ entity);

        Flood::Entity^ FindEntity(System::String^ name);

        virtual void Update(float delta) override;

        virtual bool Equals(System::Object^ object) override;

        virtual int GetHashCode() override;
    };
}
