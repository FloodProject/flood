/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"

#ifdef ENABLE_NETWORKING_CURL

#include "Core/Stream.h"
#include "Core/Memory.h"
#include "Core/Log.h"

#define CURL_STATICLIB
#include <curl/curl.h>

NAMESPACE_CORE_BEGIN

//-----------------------------------//

struct API_CORE WebStream : Stream
{
	CURL* handle;
	MemoryStream ms;
	bool downloaded;
};

static bool  WebStreamOpen(Stream*);
static bool  WebStreamClose(Stream*);
static int64 WebStreamRead(Stream*, void*, int64);
static int64 WebStreamWrite(Stream*, void*, int64);
static int64 WebStreamGetSize(Stream* s);

static StreamFuncs gs_WebFuncs = 
{
	WebStreamOpen,
	WebStreamClose,
	WebStreamRead,
	WebStreamWrite,
	0/*WebStreamTell*/,
	0/*WebStreamSeek*/,
	WebStreamGetSize,
	0/*WebStreamResize*/
};

//-----------------------------------//

static bool g_InitializedCURL = false;

static void CleanupCURL()
{
	curl_global_cleanup();
	g_InitializedCURL = false;
}

static void InitCURL()
{
	if( g_InitializedCURL ) return;
	
	curl_global_init(CURL_GLOBAL_WIN32);
	atexit(CleanupCURL);
	g_InitializedCURL = true;
}

static size_t HandleHTTP(void* ptr, size_t size, size_t nmemb, void* userdata);
static size_t HandleProgress(void *clientp, double, double, double, double);

//-----------------------------------//

Stream* StreamCreateWeb(Allocator* alloc, const String& URL, StreamMode::Enum mode)
{
	WebStream* ws = Allocate(alloc, WebStream);
	if( !ws ) return nullptr;
	
	ws->handle = nullptr;
	ws->path = URL;
	ws->mode = mode;
	ws->fn = &gs_WebFuncs;

	if( !WebStreamOpen(ws) )
	{
		Deallocate(ws);
		return nullptr;
	}

	return ws;
}

//-----------------------------------//

static bool WebStreamOpen(Stream* s)
{
	WebStream* ws = (WebStream*) s;
	if( !ws ) return false;

	InitCURL();

	ws->handle = curl_easy_init();

	if( !ws->handle )
	{
		LogError("Could not initialize cURL");
		return false;
	}

	ws->downloaded = false;

	curl_easy_setopt(ws->handle, CURLOPT_FOLLOWLOCATION, true);
	curl_easy_setopt(ws->handle, CURLOPT_URL, ws->path.c_str());
	curl_easy_setopt(ws->handle, CURLOPT_NOPROGRESS, 0);
	curl_easy_setopt(ws->handle, CURLOPT_PROGRESSFUNCTION, &HandleProgress);

	StreamMemoryInit(&ws->ms);

	return true;
}

//-----------------------------------//

static bool WebStreamClose(Stream* s)
{
	WebStream* ws = (WebStream*) s;
	if( !ws ) return false;

	curl_easy_cleanup(ws->handle);
	ws->handle = nullptr;

	return true;
}

//-----------------------------------//

static bool WebStreamPerform(WebStream* ws)
{
	CURL* curl = ws->handle;

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &HandleHTTP);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, ws);
	CURLcode status = curl_easy_perform(curl);

	return status == CURLE_OK;
}

//-----------------------------------//

static int64 WebStreamRead(Stream* s, void* buffer, int64 size)
{
	WebStream* ws = (WebStream*) s;
	if( !ws ) return -1;

	//StreamMemorySetRawBuffer(&ws->ms, (uint8*) buffer);

	if( !ws->downloaded && !WebStreamPerform(ws) )
		return -1;

	memcpy(buffer, ws->ms.buffer, (size_t) size);

	return ws->ms.position;
}

//-----------------------------------//

static int64 WebStreamWrite(Stream* s, void* buf, int64 size)
{
	WebStream* ws = (WebStream*) s;
	
	LogAssert("Not implemented yet");
	return 0;
}

//-----------------------------------//

static int64 WebStreamGetSize(Stream* s)
{
	WebStream* ws = (WebStream*) s;
	
	if( !ws->downloaded && !WebStreamPerform(ws) )
		return -1;

	double size;
	curl_easy_getinfo(ws->handle, CURLINFO_SIZE_DOWNLOAD, &size);

	return (int64) size;
}

//-----------------------------------//

static size_t HandleHTTP(void* ptr, size_t size, size_t nmemb, void* userdata)
{
	WebStream* ws = (WebStream*) userdata;
	MemoryStream* ms = &ws->ms;
	ws->downloaded = true;

	size_t total = size*nmemb;
	int64 written = StreamWrite(ms, (uint8*) ptr, total);

	return (size_t) written;
}

//-----------------------------------//

static size_t HandleProgress(void *clientp, double dltotal, double dlnow,
	double ultotal, double ulnow)
{
	return 0;
}

//-----------------------------------//

NAMESPACE_CORE_END

#endif