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

bool Flood::Network::NetworkInitialize()
{
    auto __ret = ::NetworkInitialize();
    return __ret;
}

void Flood::Network::NetworkDeinitialize()
{
    ::NetworkDeinitialize();
}

Flood::Allocator^ Flood::Network::AllocatorGetNetwork()
{
    auto __ret = ::AllocatorGetNetwork();
    if (__ret == nullptr) return nullptr;
    return (__ret == nullptr) ? nullptr : gcnew Flood::Allocator((::Allocator*)__ret);
}

