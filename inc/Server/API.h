/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

//---------------------------------------------------------------------//
// API Includes
//---------------------------------------------------------------------//

#include "Core/API.h"
#include "Core/Log.h"

#include "Network/Host.h"
#include "Network/Session.h"
#include "Network/Message.h"
#include "Network/MessageDispatcher.h"

#include "Resources/API.h"

//---------------------------------------------------------------------//
// Server APIs
//---------------------------------------------------------------------//

#define ENABLE_ENET_NETWORKING

//---------------------------------------------------------------------//
// API Exports
//---------------------------------------------------------------------//

#if defined(API_SERVER_DLL) && defined(API_SERVER_DLL_EXPORT)
	#define API_SERVER API_EXPORT
#elif defined(API_SERVER_DLL)
	#define API_SERVER API_IMPORT
#else
	#define API_SERVER
#endif

//---------------------------------------------------------------------//
// API Namespaces
//---------------------------------------------------------------------//

#define NAMESPACE_SERVER_BEGIN NAMESPACE_CORE_BEGIN
#define NAMESPACE_SERVER_END NAMESPACE_CORE_END

//---------------------------------------------------------------------//
// Memory allocators
//---------------------------------------------------------------------//

NAMESPACE_SERVER_BEGIN

API_SERVER Allocator* AllocatorGetServer();

NAMESPACE_SERVER_END