/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <Core/Serialization.h>
#include "ResourceHandle.h"

namespace Flood
{
    enum struct ReflectionWalkType : unsigned char;
    ref class Allocator;
    ref class Enum;
    ref class Field;
    ref class Primitive;
    ref class ReflectionHandleContext;
    ref class Serializer;
    ref class Stream;
    ref class Type;

    public enum struct ReflectionWalkType : unsigned char
    {
        Begin = 0,
        End = 1,
        Element = 2,
        ElementBegin = 3,
        ElementEnd = 4
    };

    public delegate unsigned int ReflectionDeserializeHandleFn(System::String^ _1);

    public ref class ReflectionHandleContext
    {
    public:
        property ::ReflectionHandleContext* NativePtr;

        ReflectionHandleContext(::ReflectionHandleContext* native);
        ReflectionHandleContext(System::IntPtr native);
        property Flood::ReflectionDeserializeHandleFn^ Deserialize
        {
            Flood::ReflectionDeserializeHandleFn^ get();
            void set(Flood::ReflectionDeserializeHandleFn^);
        }
    };

    public ref class Serializer
    {
    public:
        property ::Serializer* NativePtr;

        Serializer(::Serializer* native);
        Serializer(System::IntPtr native);
        Serializer();
        property Flood::Allocator^ Alloc
        {
            Flood::Allocator^ get();
            void set(Flood::Allocator^);
        }
        property Flood::Stream^ Stream
        {
            Flood::Stream^ get();
            void set(Flood::Stream^);
        }
        void Destroy();
    };
}
