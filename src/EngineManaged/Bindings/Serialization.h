/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "CppSharp.h"
#include <Core/Serialization.h>

namespace Flood
{
    enum struct ReflectionWalkType : unsigned char;
    enum struct SerializerType;
    ref class Allocator;
    ref class ReflectionHandleContext;
    ref class Stream;
}

namespace Flood
{
    public enum struct ReflectionWalkType : unsigned char
    {
        Begin = 0,
        End = 1,
        Element = 2,
        ElementBegin = 3,
        ElementEnd = 4
    };

    public enum struct SerializerType
    {
        Json = 0,
        Binary = 1
    };

    public ref class ReflectionHandleContext : ICppInstance
    {
    public:

        property ::ReflectionHandleContext* NativePtr;
        property System::IntPtr __Instance
        {
            virtual System::IntPtr get();
            virtual void set(System::IntPtr instance);
        }

        ReflectionHandleContext(::ReflectionHandleContext* native);
        ReflectionHandleContext(System::IntPtr native);
        ReflectionHandleContext();

        virtual bool Equals(System::Object^ object) override;

        virtual int GetHashCode() override;
    };
}
