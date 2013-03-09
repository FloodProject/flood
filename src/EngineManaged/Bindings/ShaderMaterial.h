/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <Graphics/Resources/ShaderMaterial.h>
#include "Resource.h"
#include "ResourceHandle.h"

namespace Flood
{
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
    public ref class ShaderMaterial : Flood::Resource
    {
    public:
        ShaderMaterial(::ShaderMaterial* native);
        ShaderMaterial(System::IntPtr native);
        System::String^ GetVertexSource();
        System::String^ GetFragmentSource();
        Flood::ResourceGroup GetResourceGroup();
    };
}
