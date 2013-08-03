/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Core/API.h"
#include "Core/Stream.h"
#include "Core/Memory.h"
#include "Core/Pointers.h"

#include <UnitTest++.h>

SUITE(Core)
{
	TEST(FileStreams)
	{
		// File not found
		FileStream notFound("Test.h", StreamOpenMode::Read);
		CHECK(!notFound.isValid);

		// File opening
		FileStream file("file.txt", StreamOpenMode::Read);
		CHECK(file.isValid);

		// File size.
		int64 size = file.size();
		CHECK_EQUAL(6, (int) size);

		// File read
		String text;
		file.readString(text);
		CHECK_EQUAL("foobar", text.c_str());

		// File seek
		file.setPosition(-3, StreamSeekMode::RelativeEnd);

		// File tell
		int64 offset = file.getPosition();
		CHECK_EQUAL(3, (int) offset);

		// File read
		file.readString(text);
		CHECK_EQUAL("bar", text.c_str());

		FileStream flines("lines.txt", StreamOpenMode::Read);
		CHECK(flines.isValid);

		// Read lines.
		std::vector<String> lines;
		flines.readLines(lines);
		CHECK_EQUAL(3, lines.size());
		CHECK_EQUAL("foo", lines[0].c_str());
		CHECK_EQUAL("bar", lines[1].c_str());
		CHECK_EQUAL("spam", lines[2].c_str());
	}

#if defined(ENABLE_NETWORKING_CURL)
	TEST(WebStreams)
	{
		WebStream ws("http://www.google.com", StreamOpenMode::Read);

		String response;
		ws.readString(response);

		CHECK(response.size() > 0);
	}
#endif
}