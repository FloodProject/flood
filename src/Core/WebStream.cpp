/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
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

//-----------------------------------//

WebStream::WebStream(const UTF8String& URL, StreamOpenMode mode)
	: Stream(URL, mode)
	, handle(nullptr)
{
	open(); 
}

//-----------------------------------//

WebStream::~WebStream()
{
	if( !close() )
		LogDebug("Error closing web stream: %s", path.CString());
}

//-----------------------------------//

static size_t HandleProgress(void *clientp, double dltotal, double dlnow,
	double ultotal, double ulnow)
{
	return 0;
}

//-----------------------------------//

bool WebStream::open()
{
	InitCURL();

	handle = curl_easy_init();

	if( !handle )
	{
		LogError("Could not initialize cURL");
		return false;
	}

	isPerformDone = false;

	curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, true);
	curl_easy_setopt(handle, CURLOPT_URL, path.CString());
	curl_easy_setopt(handle, CURLOPT_NOPROGRESS, 0);
	curl_easy_setopt(handle, CURLOPT_PROGRESSFUNCTION, &HandleProgress);

	ms.init();

	return true;
}

//-----------------------------------//

bool WebStream::close()
{
	curl_easy_cleanup(handle);
	handle = nullptr;

	return true;
}

//-----------------------------------//

static size_t HandleHTTP(void* ptr, size_t size, size_t nmemb, void* data)
{
	auto ws = (WebStream*) data;
	ws->isPerformDone = true;

	size_t total = size * nmemb;
	int64 written = ws->ms.write((uint8*) ptr, total);

	return (size_t) written;
}

//-----------------------------------//

bool WebStream::perform() const
{
	CURL* curl = handle;

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &HandleHTTP);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
	CURLcode status = curl_easy_perform(curl);

	return status == CURLE_OK;
}

//-----------------------------------//
uint64 WebStream::read(void* buffer, uint64 size) const
{
	//StreamMemorySetRawBuffer(&ws->ms, (uint8*) buffer);

	if (!isPerformDone && !perform())
		return InvalidState;

	if (ms.size() < size)
		size = ms.size();

	memcpy(buffer, ms.buffer, (size_t) size);

	return ms.position;
}

//-----------------------------------//

uint64 WebStream::write(void* buf, uint64 size)
{
	LogAssert("Not implemented yet");
	return 0;
}

//-----------------------------------//

uint64 WebStream::size() const
{
	if( !isPerformDone && !perform() )
		return InvalidState;

	double size;
	curl_easy_getinfo(handle, CURLINFO_SIZE_DOWNLOAD, &size);

	return (int64) size;
}

//-----------------------------------//



NAMESPACE_CORE_END

#endif