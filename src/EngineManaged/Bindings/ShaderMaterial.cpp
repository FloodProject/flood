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
    : Flood::Resource((::Resource*)native)
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
    auto obj = dynamic_cast<ShaderMaterial^>(object);

    if (!obj) return false;
    return Instance == obj->Instance;
}

int Flood::ShaderMaterial::GetHashCode()
{
    return (int)NativePtr;
}

System::String^ Flood::ShaderMaterial::VertexSource::get()
{
    auto __ret = ((::ShaderMaterial*)NativePtr)->getVertexSource();
    return clix::marshalString<clix::E_UTF8>(__ret);
}

System::String^ Flood::ShaderMaterial::FragmentSource::get()
{
    auto __ret = ((::ShaderMaterial*)NativePtr)->getFragmentSource();
    return clix::marshalString<clix::E_UTF8>(__ret);
}

Flood::ResourceGroup Flood::ShaderMaterial::ResourceGroup::get()
{
    auto __ret = ((::ShaderMaterial*)NativePtr)->getResourceGroup();
    return (Flood::ResourceGroup)__ret;
}

