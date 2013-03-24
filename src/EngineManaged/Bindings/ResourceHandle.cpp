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
    auto resource = (::Resource*)ResourceHandleFind(Id);

    auto ctor = T::typeid->GetConstructor(gcnew cli::array<System::Type^>{System::IntPtr::typeid});
    return (T) ctor->Invoke(gcnew cli::array<System::Object^>{System::IntPtr(resource)});
}

generic<typename T>
void Flood::ResourceHandle<T>::AddReference()
{
    auto resource = ResourceHandleFind(Id);
    if (resource)
        resource->addReference();
}

generic<typename T>
void Flood::ResourceHandle<T>::RemoveReference()
{
    auto resource = ResourceHandleFind(Id);
    if (resource)
        resource->releaseReference();
}

