/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <Graphics/Resources/ShaderMaterial.h>
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
    ref class ShaderMaterial;
    enum struct ResourceGroup;
    ref class Handle;
    ref class RefPtr;

    /// <summary>
    /// Shaders are used to program the GPU programmable rendering pipeline. It is
    /// a set of software instructions that instruct how to calculate each vertex
    /// and each pixel processed by the graphics card. We only deal with high-level
    /// shaders in the engine, which are compiled to low-level assembly code by the
    /// graphics driver.
    /// </summary>
    public ref class ShaderMaterial
    {
    public:
        property ::ShaderMaterial* NativePtr;

        ShaderMaterial(::ShaderMaterial* native);
        ShaderMaterial(System::IntPtr native);
        Flood::Class^ GetType();
        Flood::Class^ GetStaticType();
        System::String^ GetVertexSource();
        System::String^ GetFragmentSource();
        Flood::ResourceGroup GetResourceGroup();
    };
}
