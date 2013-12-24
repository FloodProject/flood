/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "CppSharp.h"
#include <Graphics/IndexBuffer.h>
#include "Buffer.h"

namespace Flood
{
    ref class GeometryBuffer;
    ref class IndexBuffer;
}

namespace Flood
{
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

        virtual bool Equals(System::Object^ object) override;

        virtual int GetHashCode() override;
    };
}
