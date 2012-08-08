//////////////////////////////////////////////////////////////////////////
//
// Flood © (2008-2013)
// CoreStreams - Converted Streams tests from vapor3d
//
//////////////////////////////////////////////////////////////////////////

#include "Core/API.h"
#include "UnitTest++.h"

#include "Core/Stream.h"
#include "Core/Memory.h"
#include "Core/Pointers.h"

SUITE(Core)
{
	TEST(FileStreams)
	{
		// File not found
		Stream* notFound = StreamCreateFromFile( AllocatorGetHeap(), "Test.h", StreamMode::Read );
		CHECK(nullptr == notFound);

		// File opening
		StreamPtr file( pStreamCreateFromFile(AllocatorGetHeap(), "file.txt", StreamMode::Read) );
		CHECK(nullptr != file);

		// File size.
		int64 size = StreamGetSize(file);
		CHECK_EQUAL(6, (int) size);

		// File read
		String text;
		StreamReadString(file, text);
		CHECK_EQUAL("foobar", text.c_str());

		// File seek
		StreamSetPosition(file, -3, StreamSeekMode::RelativeEnd);

		// File tell
		int64 offset = StreamGetPosition(file);
		CHECK_EQUAL(3, (int) offset);

		// File read
		StreamReadString(file, text);
		CHECK_EQUAL("bar", text.c_str());

		StreamPtr flines( pStreamCreateFromFile(AllocatorGetHeap(), "lines.txt", StreamMode::Read) );
		CHECK(nullptr != flines);

		// Read lines.
		std::vector<String> lines;
		StreamReadLines(flines, lines);
		CHECK_EQUAL(3, lines.size());
		CHECK_EQUAL("foo", lines[0].c_str());
		CHECK_EQUAL("bar", lines[1].c_str());
		CHECK_EQUAL("spam", lines[2].c_str());
	}

	TEST(WebStreams)
	{
		Stream* ws = StreamCreateWeb(AllocatorGetHeap(), "http://www.google.com", StreamMode::Read);

		String response;
		StreamReadString(ws, response);

		CHECK(response.size() > 0);

		StreamDestroy(ws);
	}
}