/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "CppSharp.h"
#include <Graphics/Resources/ShaderMaterial.h>
#include "Resource.h"
#include "ResourceHandle.h"

namespace Flood
{
    enum struct ResourceGroup;

    /// <summary>
    /// Shaders are used to program the GPU programmable rendering pipeline. It is
    /// a set of software instructions that instruct how to calculate each vertex
    /// and each pixel processed by the graphics card. We only deal with high-level
    /// shaders in the engine, which are compiled to low-level assembly code by the
    /// graphics driver.
    /// </summary>
    public ref class ShaderMaterial : Flood::Resource
    {
    public:

        ShaderMaterial(::ShaderMaterial* native);
        ShaderMaterial(System::IntPtr native);
        property System::String^ VertexSource
        {
            System::String^ get();
        }
        property System::String^ FragmentSource
        {
            System::String^ get();
        }
        property Flood::ResourceGroup ResourceGroup
        {
            Flood::ResourceGroup get();
        }
        virtual bool Equals(System::Object^ object) override;

        virtual int GetHashCode() override;

    };
}
