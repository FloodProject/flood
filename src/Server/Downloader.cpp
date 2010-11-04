/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core.h"
#include "vapor/net/Downloader.h"

#define CURL_STATICLIB
#include <curl/curl.h>

namespace vapor {

//-----------------------------------//

bool Downloader::init()
{
	CURLcode status = curl_global_init(CURL_GLOBAL_ALL);

	if( status == -1 )
	{
		Log::error("Error initializing cURL");
		return false;
	}

	Log::info("Initialized cURL (%s)", curl_version());
	return true;
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

void Downloader::download(const std::string& url)
{
	std::vector<byte> data;
	data.reserve(4096);

	CURL* curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writeHTTP);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
	curl_easy_perform(curl);
	curl_easy_cleanup(curl);

	std::string html( data.begin(), data.end() );
	Log::info("%s", html.c_str());
}

//-----------------------------------//

} // end namespace