/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Core/Event.h"

struct mg_context;
struct mg_connection;

NAMESPACE_EXTERN_BEGIN

//-----------------------------------//

struct WebContext
{
	mg_context* mongoose;
};

struct WebRequest
{
};

typedef Delegate1<WebContext*, WebRequest*> WebCallback;

API_CORE WebContext* WebServerCreate( Allocator* );
API_CORE bool WebServerStart( WebContext*, const char* port );
API_CORE void WebServerCallback( String uri, WebCallback callback );

//-----------------------------------//

NAMESPACE_EXTERN_END