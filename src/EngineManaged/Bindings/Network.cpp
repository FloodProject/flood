/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Network.h"

using namespace System;
using namespace System::Runtime::InteropServices;

bool Flood::FloodNetwork::NetworkInitialize()
{
    auto ret = ::NetworkInitialize();
    return ret;
}

void Flood::FloodNetwork::NetworkDeinitialize()
{
    ::NetworkDeinitialize();
}

