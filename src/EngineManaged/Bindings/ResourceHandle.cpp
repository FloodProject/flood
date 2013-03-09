/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "ResourceHandle.h"
#include "Resource.h"

generic<typename T> where T : Flood::Resource
Flood::ResourceHandle<T> Flood::ResourceHandle<T>::Create(T resource)
{
    auto handle = ResourceHandleCreate(resource->NativePtr);
    return Flood::ResourceHandle<T>(handle.getId());
}