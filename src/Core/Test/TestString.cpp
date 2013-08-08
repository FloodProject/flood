	/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#define INSTANTIATE_TEMPLATES

#include "Core/API.h"
#include "Core/String.h"
#include <UnitTest++.h>

SUITE(Core)
{
	TEST(Strings)
	{
		int ret = StringCompareInsensitive("Test Compare CASES", "test compare cases"); 
		CHECK_EQUAL(ret, 0);

		String str = StringFormat("%d %s", 20, "simple");
		CHECK_EQUAL("20 simple", str.c_str());

		std::vector<String> elems;
		StringSplit("Foo|Bar|Spam", '|', elems);
		CHECK_EQUAL(elems.size(), 3);
		CHECK_EQUAL("Foo", elems[0].c_str());
		CHECK_EQUAL("Bar", elems[1].c_str());
		CHECK_EQUAL("Spam", elems[2].c_str());

		str = StringToLowerCase("Foo");
		CHECK_EQUAL("foo", str.c_str());

		str = StringToUpperCase("Foo");
		CHECK_EQUAL("FOO", str.c_str());

		str = "Testing <match>";
		StringReplace(str, "<match>", "rocks");
		CHECK_EQUAL("Testing rocks", str.c_str());

		str = StringFromFloat(1.3371230f, 3);
		CHECK_EQUAL("1.337", str.c_str());

		str = StringTrim("       Trim       ", " ");
		CHECK_EQUAL("Trim", str.c_str());
	}

	TEST(Paths)
	{
		Path path = PathGetFileBase("File.cpp"); 
		CHECK_EQUAL(path, Path("File"));

		path = PathGetFileBase("Long/Path/To/File.cpp");
		CHECK_EQUAL(Path("File"), path);

		path = PathGetFileExtension("Long/Path/To/File.cpp"); 
		CHECK_EQUAL(path, Path("cpp"));

		path = PathGetFileExtension("File.cpp"); 
		CHECK_EQUAL(path, Path("cpp"));

		path = PathGetBase("Long/Path/To/File"); 
		CHECK_EQUAL(path, Path("Long/Path/To/"));

		path = PathGetBase("/Absolute/Path/To/File"); 
		CHECK_EQUAL(path, Path("/Absolute/Path/To/"));

		path = PathGetBase("/File"); 
		CHECK_EQUAL(path, Path("/"));

		path = PathGetFile("Long/Path/To/File"); 
		CHECK_EQUAL(path, Path("File"));

		path = PathGetFile("\\Long\\Path\\To\\File"); 
		CHECK_EQUAL(path, Path("File"));

		path = PathGetFile("/Absolute/Path/To/File"); 
		CHECK_EQUAL(path, Path("File"));

		path = PathGetFile("/File"); 
		CHECK_EQUAL(path, Path("File"));

		path = PathNormalize("/Weird\\Path/To\\File"); 
		CHECK_EQUAL(path, Path("/Weird/Path/To/File"));

		path = PathNormalize("/Path/To/../File");
		CHECK_EQUAL(path, Path("/Path/To/../File"));

#ifdef PLATFORM_WINDOWS
		CHECK_EQUAL("\\", PathGetSeparator().c_str());
#else
		CHECK_EQUAL("/", PathGetSeparator().c_str());
#endif

		// TODO: Add path combine tests.
	}
}