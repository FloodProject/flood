/************************************************************************
*
* Flood Project � (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#ifdef ENABLE_HTTP_SERVER

#include "Core/Event.h"
#include "Core/Containers/Vector.h"

struct mg_context;
struct mg_connection;
struct mg_request_info;

NAMESPACE_CORE_BEGIN

//-----------------------------------//

struct WebRequest
{
	mg_connection* conn;
	const mg_request_info* req;
};

struct WebContext;
typedef Delegate2<WebContext*, WebRequest*> WebCallback;

struct WebRoute
{
	String pattern;
	WebCallback callback;
};

struct WebContext
{
	mg_context* mongoose;
	Vector<WebRoute> routes;
};

API_CORE WebContext* WebServerCreate( Allocator* );
API_CORE bool WebServerStart( WebContext*, uint16 port );
API_CORE void WebServerStop( WebContext* );
API_CORE void WebServerAddRoute( WebContext*, WebRoute route );

//-----------------------------------//

NAMESPACE_CORE_END

#endif