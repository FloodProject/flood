/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"
#include "Core/WebServer.h"
#include "Core/Memory.h"

#include "mongoose.h"

NAMESPACE_EXTERN_BEGIN

//-----------------------------------//

static void* MongooseCallback(mg_event event, mg_connection* conn, const mg_request_info* request_info)
{
	return 0;
};

WebContext* WebServerCreate( Allocator* alloc )
{
	WebContext* context = Allocate(WebContext, alloc);
	context->mongoose = nullptr;

	return context;
}

bool WebServerStart( WebContext* context, const char* port )
{
	if( !context ) return false;

	mg_callback_t callback = MongooseCallback;

	const char *options[] =
	{
		"listening_ports", port,
		"num_threads", "2",
		nullptr
	};

	context->mongoose = mg_start(callback, context, options);

	return false;
}

void WebServerCallback( String uri, WebCallback callback )
{
}

//-----------------------------------//

NAMESPACE_EXTERN_END