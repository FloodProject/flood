/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
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