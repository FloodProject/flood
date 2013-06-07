/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <Graphics/IndexBuffer.h>
#include "Buffer.h"
#include "ResourceHandle.h"

namespace Flood
{
    ref class Buffer;
    ref class GeometryBuffer;
    ref class IndexBuffer;
    ref class RefPtr;

    public ref class IndexBuffer : Flood::Buffer
    {
    public:
        IndexBuffer(::IndexBuffer* native);
        IndexBuffer(System::IntPtr native);
        IndexBuffer();
        property bool IsBuilt
        {
            bool get();
            void set(bool);
        }
    };
}
