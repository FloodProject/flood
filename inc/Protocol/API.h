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
#include "Core/References.h"
#include "Network/Message.h"

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

FWD_DECL_INTRUSIVE(Message)
FWD_DECL_INTRUSIVE(Session)

//---------------------------------------------------------------------//
// Macro declarations
//---------------------------------------------------------------------//

#define PROTOCOL_PLUGIN_BEGIN(name) \
	PluginMetadata name::getMetadata() { \
	PluginMetadata metadata;

#define METADATA_NAME(n) metadata.name = #n;
#define METADATA_DESC(d) metadata.description = #d;
#define METADATA_AUTHOR(a) metadata.author = #a;
#define METADATA_VERSION(v) metadata.version = #v;
#define METADATA_PRIORITY(p) metadata.priority = p;

#define PROTOCOL_PLUGIN_END() return metadata; }

#define PROTOCOL_MESSAGE_HANDLERS(name) \
	static MessageMapping gs_##name##RawMessages[] = {

#define PROTOCOL_MESSAGE_HANDLERS_END() };

#define IMPLEMENT_HANDLER_REF(name, msg) \
	static void Handle##msg(MessagePlugin* mp, \
	const SessionPtr& session, Object* object) { \
	name##MessagePlugin* smp = (name##MessagePlugin*) mp; \
	msg##Message* message = (msg##Message*) object; \
	smp->handle##msg(session, *message); }

#define HANDLER_RAW(name, msg, dir) \
	{ name##MessageIds::msg, MessageDirection::dir, 0, \
	(MessageRawHandlerMemFn) &name##MessagePlugin::handle##msg },

#define HANDLER_REF(name, msg, dir) \
	{ name##MessageIds::msg, MessageDirection::dir, Handle##msg, 0 },