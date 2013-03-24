/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <Core/Handle.h>

namespace Flood
{
    ref class Resource;

    generic<typename T> where T : Flood::Resource
    public value class ResourceHandle sealed
    {
    public:

        static uint32 Invalid = 0;

        // Handle id of the native resource.
        uint32 Id;

        ResourceHandle(uint32 handleId) : Id(handleId)
        {
            AddReference();
        }

        T Resolve();
        void AddReference();
        void RemoveReference();

#if RESOURCE_HANDLE_REF_CLASS
        ResourceHandle() : Id(Invalid)
        {
        }

        ResourceHandle(const ResourceHandle<T>% handle) : Id(handle.Id)
        {
        }

        ResourceHandle<T>% operator=(const ResourceHandle<T>% handle)
        {
            Id = handle.Id;
            return *this;
        }
#endif

        static ResourceHandle<T> Create(T resource);
    };
}