/************************************************************************
*
* vapor3D Server © (2008-2010)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

NAMESPACE_CORE_BEGIN

//-----------------------------------//

API_CORE bool NetworkInitialize();
API_CORE void NetworkDeinitialize();

API_CORE Allocator* AllocatorGetNetwork();

//-----------------------------------//

NAMESPACE_CORE_END