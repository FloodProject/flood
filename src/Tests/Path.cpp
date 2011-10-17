/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "CuTest.h"

#include "Core/API.h"
#include "Core/String.h"

void TestString(CuTest* tc)
{
	int ret = StringCompareInsensitive("Test Compare CASES", "test compare cases"); 
	CuAssertIntEquals(tc, ret, 0);

	String str = StringFormat("%d %s", 20, "simple");
	CuAssertStrEquals(tc, "20 simple", str.c_str());

	std::vector<String> elems;
	StringSplit("Foo|Bar|Spam", '|', elems);
	CuAssertIntEquals(tc, elems.size(), 3);
	CuAssertStrEquals(tc, "Foo", elems[0].c_str());
	CuAssertStrEquals(tc, "Bar", elems[1].c_str());
	CuAssertStrEquals(tc, "Spam", elems[2].c_str());

	str = StringToLowerCase("Foo");
	CuAssertStrEquals(tc, "foo", str.c_str());

	str = StringToUpperCase("Foo");
	CuAssertStrEquals(tc, "FOO", str.c_str());

	str = "Testing <match>";
	StringReplace(str, "<match>", "rocks");
	CuAssertStrEquals(tc, "Testing rocks", str.c_str());

	str = StringFromFloat(1.3371230f, 3);
	CuAssertStrEquals(tc, "1.337", str.c_str());

	str = StringTrim("       Trim       ", " ");
	CuAssertStrEquals(tc, "Trim", str.c_str());
}

void TestPath(CuTest* tc)
{
	Path path = PathGetFileBase("File.cpp"); 
	CuAssertCppStrEquals(tc, path, Path("File"));

	path = PathGetFileBase("Long/Path/To/File.cpp");
	CuAssertCppStrEquals(tc, Path("File"), path);

	path = PathGetFileExtension("Long/Path/To/File.cpp"); 
	CuAssertCppStrEquals(tc, path, Path("cpp"));

	path = PathGetFileExtension("File.cpp"); 
	CuAssertCppStrEquals(tc, path, Path("cpp"));

	path = PathGetBase("Long/Path/To/File"); 
	CuAssertCppStrEquals(tc, path, Path("Long/Path/To/"));

	path = PathGetBase("/Absolute/Path/To/File"); 
	CuAssertCppStrEquals(tc, path, Path("/Absolute/Path/To/"));

	path = PathGetBase("/File"); 
	CuAssertCppStrEquals(tc, path, Path("/"));

	path = PathGetFile("Long/Path/To/File"); 
	CuAssertCppStrEquals(tc, path, Path("File"));

	path = PathGetFile("\\Long\\Path\\To\\File"); 
	CuAssertCppStrEquals(tc, path, Path("File"));

	path = PathGetFile("/Absolute/Path/To/File"); 
	CuAssertCppStrEquals(tc, path, Path("File"));

	path = PathGetFile("/File"); 
	CuAssertCppStrEquals(tc, path, Path("File"));

	path = PathNormalize("/Weird\\Path/To\\File"); 
	CuAssertCppStrEquals(tc, path, Path("/Weird/Path/To/File"));

	path = PathNormalize("/Path/To/../File");
	CuAssertCppStrEquals(tc, path, Path("/Path/To/../File"));

#ifdef PLATFORM_WINDOWS
	CuAssertStrEquals(tc, "\\", PathGetSeparator().c_str());
#else
	CuAssertStrEquals(tc, "/", PathGetSeparator().c_str());
#endif

	// TODO: Add path combine tests.
}

CuSuite* GetSuitePath()
{
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, TestString);
	SUITE_ADD_TEST(suite, TestPath);
	return suite;
}