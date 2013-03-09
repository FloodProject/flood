/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "_Marshal.h"
#include "ShaderMaterial.h"
#include "Resource.h"
#include "ResourceHandle.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace clix;

Flood::ShaderMaterial::ShaderMaterial(::ShaderMaterial* native)
    : Resource(native)
{
}

Flood::ShaderMaterial::ShaderMaterial(System::IntPtr native)
    : Resource(native)
{
    auto __native = (::ShaderMaterial*)native.ToPointer();
}

System::String^ Flood::ShaderMaterial::GetVertexSource()
{
    auto ret = ((::ShaderMaterial*)NativePtr)->getVertexSource();
    return clix::marshalString<clix::E_UTF8>(ret);
}

System::String^ Flood::ShaderMaterial::GetFragmentSource()
{
    auto ret = ((::ShaderMaterial*)NativePtr)->getFragmentSource();
    return clix::marshalString<clix::E_UTF8>(ret);
}

Flood::ResourceGroup Flood::ShaderMaterial::GetResourceGroup()
{
    auto ret = ((::ShaderMaterial*)NativePtr)->getResourceGroup();
    return (Flood::ResourceGroup)ret;
}

