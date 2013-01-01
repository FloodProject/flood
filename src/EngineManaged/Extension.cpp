/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Extension.h"

using namespace System;
using namespace System::Runtime::InteropServices;

Flood::Extension::Extension(::Extension* native)
{
    NativePtr = native;
}

Flood::Extension::Extension()
{
}

Flood::Class^ Flood::Extension::GetType()
{
    return nullptr;
}

Flood::Class^ Flood::Extension::GetStaticType()
{
    return nullptr;
}

Flood::ExtensionMetadata Flood::Extension::GetMetadata()
{
    return ExtensionMetadata();
}

void Flood::Extension::OnInit()
{
}

void Flood::Extension::OnCleanup()
{
}

Flood::Class^ Flood::FloodExtension::ExtensionGetType()
{
    return nullptr;
}


