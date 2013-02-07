/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <Graphics/IndexBuffer.h>

namespace Flood
{
    ref class GeometryBuffer;
    ref class IndexBuffer;
    ref class RefPtr;

    public ref class IndexBuffer
    {
    public:
        property ::IndexBuffer* NativePtr;

        IndexBuffer(::IndexBuffer* native);
        IndexBuffer(System::IntPtr native);
        property bool IsBuilt;
        IndexBuffer();
    };
}
