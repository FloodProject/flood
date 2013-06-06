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
    ref class Type;

    public enum struct ReflectionWalkType : unsigned char
    {
        Begin = 0,
        End = 1,
        Element = 2,
        ElementBegin = 3,
        ElementEnd = 4
    };

    public ref class ReflectionHandleContext
    {
    public:
        property ::ReflectionHandleContext* NativePtr;

        ReflectionHandleContext(::ReflectionHandleContext* native);
        ReflectionHandleContext(System::IntPtr native);
        ReflectionHandleContext();
    };

}
