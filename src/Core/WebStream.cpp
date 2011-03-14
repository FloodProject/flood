/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"
#include "WebStream.h"
#include "Log.h"

#ifdef VAPOR_NETWORKING_CURL

#define CURL_STATICLIB
#include <curl/curl.h>

namespace vapor {

//-----------------------------------//

static size_t writeHTTP(void* ptr, size_t size, size_t nmemb, void* userdata);

//-----------------------------------//

WebStream::WebStream(const String& url, StreamMode::Enum mode)
	: Stream(StreamMode::Read, url)
{ }

//-----------------------------------//

bool WebStream::open()
{
	if( request != nullptr )
		return false;

	request = curl_easy_init();
	curl_easy_setopt(request, CURLOPT_URL, path.c_str());
	
	return true;
}

//-----------------------------------//

void WebStream::close()
{
	curl_easy_cleanup(request);
	request = nullptr;
}

//-----------------------------------//

long WebStream::read(void* buffer, long size) const
{
	return 0;
}

//-----------------------------------//

void WebStream::read(std::vector<byte>& data) const
{
	data.reserve(4096);

	curl_easy_setopt(request, CURLOPT_WRITEFUNCTION, &writeHTTP);
	curl_easy_setopt(request, CURLOPT_WRITEDATA, &data);
	curl_easy_perform(request);

	String response( data.begin(), data.end() );
	LogInfo("%s", response.c_str());
}

//-----------------------------------//

long WebStream::write(const std::vector<byte>& buffer)
{
	return 0;
}

//-----------------------------------//

static size_t writeHTTP(void* ptr, size_t size, size_t nmemb, void* userdata)
{
	std::vector<byte>& data = *(std::vector<byte>*) userdata;
	
	int more = size*nmemb;
	size_t sz = data.size();
	
	data.resize( sz + more );
	std::copy( (byte*)ptr, (byte*)ptr+more, data.begin()+sz );

	return more;
}

//-----------------------------------//

} // end namespace

#endif