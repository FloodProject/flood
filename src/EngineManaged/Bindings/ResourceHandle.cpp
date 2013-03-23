/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "ResourceHandle.h"
#include "Resource.h"

generic<typename T>
Flood::ResourceHandle<T> Flood::ResourceHandle<T>::Create(T resource)
{
    auto handle = ResourceHandleCreate(resource->NativePtr);
    return Flood::ResourceHandle<T>(handle.getId());
}

generic<typename T>
T Flood::ResourceHandle<T>::Resolve()
{
    auto resource = ResourceHandleFind(Id);

    auto instance = System::Activator::CreateInstance<T>();
    instance->NativePtr = (::Resource*)resource;

    return instance;
}
