	/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#define INSTANTIATE_TEMPLATES

#include "Core/API.h"
#include "Core/String.h"
#include "Core/Containers/Str.h"
#include <UnitTest++.h>

SUITE(Core)
{
	TEST(Strings)
	{
		int ret = String::Compare("Test Compare CASES", "test compare cases", false); 
		CHECK_EQUAL(ret, 0);

		String str = StringFormat("%d %s", 20, "simple");
		CHECK_EQUAL("20 simple", str.CString());

		Vector<String> elems = String::Split("Foo|Bar|Spam", '|');
		CHECK_EQUAL(elems.Size(), 3);
		CHECK_EQUAL("Foo", elems[0].CString());
		CHECK_EQUAL("Bar", elems[1].CString());
		CHECK_EQUAL("Spam", elems[2].CString());

		str = String("Foo").ToLower();
		CHECK_EQUAL("foo", str.CString());

		str = String("Foo").ToUpper();
		CHECK_EQUAL("FOO", str.CString());

		str = "Testing <match>";
		str.Replace("<match>", "rocks");
		CHECK_EQUAL("Testing rocks", str.CString());

		str = StringFromFloat(1.3371230f, 3);
		CHECK_EQUAL("1.337", str.CString());

		str = StringTrim("       Trim       ", " ");
		CHECK_EQUAL("Trim", str.CString());
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
		CHECK_EQUAL("\\", PathGetSeparator().CString());
#else
		CHECK_EQUAL("/", PathGetSeparator().CString());
#endif

		// TODO: Add path combine tests.
	}
}