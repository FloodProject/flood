/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "_Marshal.h"
#include "Network.h"
#include "ResourceHandle.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace clix;

bool Flood::FloodNetwork::NetworkInitialize()
{
    auto ret = ::NetworkInitialize();
    return ret;
}

void Flood::FloodNetwork::NetworkDeinitialize()
{
    ::NetworkDeinitialize();
}

