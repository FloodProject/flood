/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <Core/Extension.h>
#include "Reflection.h"
#include "Memory.h"

namespace Flood
{
    ref class Class;
    ref class Allocator;
    ref class Field;
    ref class Type;
    enum struct TypeKind : unsigned char;
    ref class ReflectionContext;
    enum struct ReflectionWalkType : unsigned char;
    enum struct FieldQualifier : unsigned short;
    value struct ExtensionMetadata;
    ref class Extension;
    ref class HandleManager;
    ref class ReferenceCounted;
    ref class Handle;
    ref class Object;
    ref class scoped_ptr;

    public value struct ExtensionMetadata
    {
    public:
        ExtensionMetadata(::ExtensionMetadata* native);
        ExtensionMetadata(System::IntPtr native);
        System::String^ Name;
        System::String^ Description;
        int Priority;
    };

    /// <summary>
    /// Extensions are clearly defined units of extensibility to applications.
    /// Typically an application will create the extension points and provide some
    /// kind of runtime support for loading the extensions from a plugin.
    /// </summary>
    public ref class Extension
    {
    public:
        property ::Extension* NativePtr;

        Extension(::Extension* native);
        Extension(System::IntPtr native);
        Extension();
        Flood::Class^ GetType();
        Flood::Class^ GetStaticType();
        Flood::ExtensionMetadata GetMetadata();
        void OnInit();
        void OnCleanup();
    };
}
