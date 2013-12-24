/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Network.h"
#include "Memory.h"

using namespace System;
using namespace System::Runtime::InteropServices;

bool Flood::FloodNetwork::NetworkInitialize()
{
    auto __ret = ::NetworkInitialize();
    return __ret;
}

void Flood::FloodNetwork::NetworkDeinitialize()
{
    ::NetworkDeinitialize();
}

Flood::Allocator^ Flood::FloodNetwork::AllocatorGetNetwork()
{
    auto __ret = ::AllocatorGetNetwork();
    if (__ret == nullptr) return nullptr;
    return gcnew Flood::Allocator((::Allocator*)__ret);
}

