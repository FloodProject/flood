/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "CuTest.h"

#include "Core/API.h"
#include "Core/Stream.h"
#include "Core/Memory.h"
#include "Core/Pointers.h"

using namespace vapor;

void TestFile(CuTest *tc)
{
	// File not found
	Stream* file = StreamCreateFromFile( AllocatorGetDefault(), "Test.h", StreamMode::Read );
	CuAssertPtrEquals(tc, nullptr, file);

	// File opening
	file = StreamCreateFromFile( AllocatorGetDefault(), "file.txt", StreamMode::Read );
	scoped_ptr<Stream> p_file( file );
	CuAssertPtrNotNull(tc, file);

	// File size.
	int64 size = StreamGetSize(file);
	CuAssertIntEquals(tc, 6, size);

	// File read
	String text;
	StreamReadString(file, text);
	CuAssertStrEquals(tc, "foobar", text.c_str());

	// File seek
	StreamSetPosition(file, -3, StreamSeekMode::RelativeEnd);

	// File tell
	int64 offset = StreamGetPosition(file);
	CuAssertIntEquals(tc, 3, offset);

	// File read
	StreamReadString(file, text);
	CuAssertStrEquals(tc, "bar", text.c_str());
}

CuSuite* GetSuiteStreams()
{
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, TestFile);
    return suite;
}