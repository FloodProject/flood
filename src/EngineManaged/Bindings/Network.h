/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "CppSharp.h"
#include <Core/Network/Network.h>

namespace Flood
{
    public ref class FloodNetwork
    {
    public:
        static bool NetworkInitialize();
        static void NetworkDeinitialize();
    };
}
