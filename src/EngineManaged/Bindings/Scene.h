/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "CppSharp.h"
#include <Engine/Scene/Scene.h>
#include "Resource.h"
#include "ResourceHandle.h"
#include "Vector.h"

namespace Flood
{
    enum struct ResourceGroup;
    ref class Entity;
    ref class Geometry;
    ref class Group;
    ref class RayQueryResult;
    ref class RayTriangleQueryResult;
    ref class RenderBatch;
    value struct Frustum;
    value struct Ray;
}

namespace Flood
{
    public ref class RayQueryResult : ICppInstance
    {
    public:

        property ::RayQueryResult* NativePtr;
        property System::IntPtr Instance
        {
            virtual System::IntPtr get();
            virtual void set(System::IntPtr instance);
        }

        RayQueryResult(::RayQueryResult* native);
        RayQueryResult(System::IntPtr native);
        RayQueryResult();

        property Flood::Entity^ Entity
        {
            Flood::Entity^ get();
            void set(Flood::Entity^);
        }

        property float Distance
        {
            float get();
            void set(float);
        }

        virtual bool Equals(System::Object^ object) override;

        virtual int GetHashCode() override;
    };

    public ref class RayTriangleQueryResult : Flood::RayQueryResult
    {
    public:

        RayTriangleQueryResult(::RayTriangleQueryResult* native);
        RayTriangleQueryResult(System::IntPtr native);
        RayTriangleQueryResult();

        property Flood::Geometry^ Geometry
        {
            Flood::Geometry^ get();
            void set(Flood::Geometry^);
        }

        property Flood::RenderBatch^ Renderable
        {
            Flood::RenderBatch^ get();
            void set(Flood::RenderBatch^);
        }

        property Flood::Vector3 IntersectionLocal
        {
            Flood::Vector3 get();
            void set(Flood::Vector3);
        }

        property Flood::Vector3 IntersectionWorld
        {
            Flood::Vector3 get();
            void set(Flood::Vector3);
        }

        property Flood::Vector3 IntersectionUV
        {
            Flood::Vector3 get();
            void set(Flood::Vector3);
        }

        virtual bool Equals(System::Object^ object) override;

        virtual int GetHashCode() override;
    };

    /// <summary>
    /// A scene instance holds all the entities that are attached to it. You might
    /// have different scenes for different 'levels' of your game and switch
    /// between them in runtime. The scene is also responsible for updating all the
    /// game entities and will aswell call hooks like before performing frustum
    /// culling or before rendering the entity.
    /// </summary>
    public ref class Scene : Flood::Resource
    {
    public:

        Scene(::Scene* native);
        Scene(System::IntPtr native);
        Scene();

        property Flood::ResourceGroup ResourceGroup
        {
            Flood::ResourceGroup get();
        }

        property Flood::Group^ Entities
        {
            Flood::Group^ get();
        }

        void Update(float delta);

        bool DoRayBoxQuery(Flood::Ray ray, Flood::RayQueryResult^ res);

        bool DoRayBoxQuery(Flood::Ray ray, System::Collections::Generic::List<Flood::RayQueryResult^>^ list, bool all);

        bool DoRayVolumeQuery(Flood::Frustum volume, System::Collections::Generic::List<Flood::RayQueryResult^>^ list, bool all);

        bool DoRayTriangleQuery(Flood::Ray ray, Flood::RayTriangleQueryResult^ res);

        bool DoRayTriangleQuery(Flood::Ray ray, Flood::RayTriangleQueryResult^ res, Flood::Entity^ _0);

        virtual bool Equals(System::Object^ object) override;

        virtual int GetHashCode() override;
    };
}
