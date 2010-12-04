/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Stream.h"

typedef void CURL;

namespace vapor {

//-----------------------------------//

/**
 * Wraps an HTTP request in an I/O stream.
 */

class CORE_API WebStream : public Stream
{
	DECLARE_UNCOPYABLE(WebStream)

public:

	WebStream(const std::string& url, StreamMode::Enum mode = StreamMode::Read);
	//virtual ~WebStream();

	// Opens the HTTP request.
	bool open();

	// Closes the HTTP request.
	void close();

	// Performs the request and returns the response data.
	long read(void* buffer, long size) const;

	// Performs the request and returns the response data.
	void read(std::vector<byte>& data) const;

	// Write buffer into the stream.
	long write(const std::vector<byte>& buf);

protected:
	
	mutable CURL* request;
};

//-----------------------------------//

} // end namespace
