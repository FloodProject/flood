/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"

#ifdef ENABLE_HTTP_SERVER

#include <cstdio>

#include "Core/String.h"
#include "Core/WebServer.h"
#include "Core/Memory.h"
#include "mongoose.h"

NAMESPACE_CORE_BEGIN

//-----------------------------------//

static void* MongooseCallback(mg_event event, mg_connection* conn, const mg_request_info* request_info)
{
	if(event != MG_NEW_REQUEST) return 0;

	WebContext* context = (WebContext*) request_info->user_data;

	// Find route callback.
	WebCallback callback = 0;

	for(size_t i = 0; i < context->routes.size(); ++i)
	{
		WebRoute& route = context->routes[i];
		
		if( !StringMatch(request_info->uri, route.pattern) )
			continue;

		callback = route.callback;
		break;
	}

	WebRequest req;
	req.conn = conn;
	req.req = request_info;

	if( !callback ) return 0;
	callback(context, &req);

	return (void*) 1;
};

//-----------------------------------//

WebContext* WebServerCreate( Allocator* alloc )
{
	WebContext* context = Allocate(alloc, WebContext);
	context->mongoose = nullptr;

	return context;
}

//-----------------------------------//

#ifdef COMPILER_MSVC
#define snprintf _snprintf
//This function or variable may be unsafe. Consider using _snprintf_s instead.
#pragma warning(disable:4996)
#endif

bool WebServerStart( WebContext* context, uint16 port )
{
	if( !context ) return false;

	static char buf[64];
	snprintf(buf, ARRAY_SIZE(buf), "%hu", port);

	const char *options[] =
	{
		"listening_ports", buf,
		"num_threads", "10",
		nullptr
	};

	mg_callback_t callback = MongooseCallback;
	context->mongoose = mg_start(callback, context, options);

	return false;
}

//-----------------------------------//

void WebServerStop( WebContext* context )
{
	mg_stop(context->mongoose);
}

//-----------------------------------//

void WebServerAddRoute( WebContext* context, WebRoute route )
{
	context->routes.push_back(route);
}

//-----------------------------------//

NAMESPACE_CORE_END

#endif