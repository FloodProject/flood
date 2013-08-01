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
