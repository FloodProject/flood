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
#include "Core/ReferenceCount.h"

//---------------------------------------------------------------------//
// API Exports
//---------------------------------------------------------------------//

#if defined(API_PROTOCOL_DLL) && defined(API_PROTOCOL_DLL_EXPORT)
	#define API_PROTOCOL API_EXPORT
#elif defined(API_PROTOCOL_DLL)
	#define API_PROTOCOL API_IMPORT
#else
	#define API_PROTOCOL
#endif

//---------------------------------------------------------------------//
// API Namespaces
//---------------------------------------------------------------------//

#define NAMESPACE_PROTOCOL_BEGIN NAMESPACE_CORE_BEGIN
#define NAMESPACE_PROTOCOL_END NAMESPACE_CORE_END

//---------------------------------------------------------------------//
// Memory allocators
//---------------------------------------------------------------------//

NAMESPACE_PROTOCOL_BEGIN

//API_PROTOCOL Allocator* AllocatorGetServer();

NAMESPACE_PROTOCOL_END

//---------------------------------------------------------------------//
// Forward declarations
//---------------------------------------------------------------------//

typedef uint32 MessageId;

FWD_DECL_INTRUSIVE(Message)
FWD_DECL_INTRUSIVE(Session)