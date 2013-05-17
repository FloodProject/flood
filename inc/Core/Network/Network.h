/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Core/API.h";

NAMESPACE_CORE_BEGIN

//-----------------------------------//

API_CORE bool NetworkInitialize();
API_CORE void NetworkDeinitialize();

API_CORE Allocator* AllocatorGetNetwork();

//-----------------------------------//

NAMESPACE_CORE_END