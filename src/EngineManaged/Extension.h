/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <Core/Extension.h>

namespace Flood
{
    ref class Class;
    value struct ExtensionMetadata;
    ref class Extension;

    public value struct ExtensionMetadata
    {
    };

    /// <summary>
    /// Extensions are clearly defined units of extensibility to applications.
    /// Typically an application will create the extension points and provide
    /// some kind of runtime support for loading the extensions from a plugin.
    /// </summary>
    public ref class Extension
    {
        property ::Extension* NativePtr;

    public:
        Extension(::Extension* native);
        Extension();
        Flood::Class^ GetType();
        Flood::Class^ GetStaticType();
        Flood::ExtensionMetadata GetMetadata();
        void OnInit();
        void OnCleanup();
    };

    public ref class FloodExtension
    {
    public:
        static Flood::Class^ ExtensionGetType ();
    };
}

