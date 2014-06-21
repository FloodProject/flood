/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "CppSharp.h"
#include <Engine/Scene/Component.h>

namespace Flood
{
    enum struct DebugDrawFlags : unsigned char;
    ref class Camera;
    ref class Component;
    ref class DebugDrawer;
    ref class Entity;
    ref class RenderBatch;
}

namespace Flood
{
    /// <summary>
    /// Represents a component, a specific piece of functionality that can be added
    /// to each entity in the scene. A component will tipically hold a piece of
    /// data and update it's state each frame if it needs to do it. A component
    /// will also be able to register methods for scripting.
    /// </summary>
    public ref class Component : ICppInstance
    {
    public:

        property ::Component* NativePtr;
        property System::IntPtr __Instance
        {
            virtual System::IntPtr get();
            virtual void set(System::IntPtr instance);
        }

        Component(::Component* native);
        Component(System::IntPtr native);
        Component();

        property Flood::Entity^ Entity
        {
            Flood::Entity^ get();
            void set(Flood::Entity^);
        }

        property bool DebugRenderableVisible
        {
            void set(bool);
        }

        property bool DebugInheritsTransform
        {
            bool get();
            void set(bool);
        }

        virtual void Update(float delta);

        virtual void OnPreRender(Flood::Camera^ camera);

        virtual bool IsDebugRenderableVisible();

        virtual void OnDebugDraw(Flood::DebugDrawer^ _0, Flood::DebugDrawFlags _1);

        virtual bool Equals(System::Object^ object) override;

        virtual int GetHashCode() override;
    };
}
