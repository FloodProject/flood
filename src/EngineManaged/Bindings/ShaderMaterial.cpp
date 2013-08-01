/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "ShaderMaterial.h"
#include "Resource.h"

using namespace System;
using namespace System::Runtime::InteropServices;

Flood::ShaderMaterial::ShaderMaterial(::ShaderMaterial* native)
    : Flood::Resource(native)
{
}

Flood::ShaderMaterial::ShaderMaterial(System::IntPtr native)
    : Flood::Resource(native)
{
    auto __native = (::ShaderMaterial*)native.ToPointer();
}

bool Flood::ShaderMaterial::Equals(System::Object^ object)
{
    if (!object) return false;
    return Instance == safe_cast<ICppInstance^>(object)->Instance;
}

int Flood::ShaderMaterial::GetHashCode()
{
    return (int)NativePtr;
}

System::String^ Flood::ShaderMaterial::VertexSource::get()
{
    auto ret = ((::ShaderMaterial*)NativePtr)->getVertexSource();
    return clix::marshalString<clix::E_UTF8>(ret);
}

System::String^ Flood::ShaderMaterial::FragmentSource::get()
{
    auto ret = ((::ShaderMaterial*)NativePtr)->getFragmentSource();
    return clix::marshalString<clix::E_UTF8>(ret);
}

Flood::ResourceGroup Flood::ShaderMaterial::ResourceGroup::get()
{
    auto ret = ((::ShaderMaterial*)NativePtr)->getResourceGroup();
    return (Flood::ResourceGroup)ret;
}

