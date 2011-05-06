/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "CuTest.h"

#include "Core/API.h"
#include "Core/Archive.h"
#include "Core/Stream.h"

using namespace vapor;

//-----------------------------------//

void TestArchiveDir(CuTest* tc)
{
	ArchivePtr archive( pArchiveCreateFromDirectory(AllocatorGetHeap(), "teste") );
	CuAssertPtrNotNull(tc, archive);

	std::vector<Path> files;
	ArchiveEnumerateFiles(archive, files);
	CuAssertIntEquals( tc, 4, files.size() );

	CuAssertTrue(tc, ArchiveExistsFile(archive, "foo.txt") );
	CuAssertTrue(tc, ArchiveExistsFile(archive, "bar.txt") );
	CuAssertTrue(tc, ArchiveExistsFile(archive, "foo/bar.txt") );
	CuAssertTrue(tc, !ArchiveExistsFile(archive, "spam.txt") );

	Stream* stream = ArchiveOpenFile(archive, "foo.txt", AllocatorGetHeap());
	CuAssertPtrNotNull(tc, stream);

	String text;
	StreamReadString(stream, text);
	StreamDestroy(stream);
	CuAssertStrEquals(tc, "foobar", text.c_str());

	std::vector<Path> dirs;
	ArchiveEnumerateDirectories(archive, dirs);
	
	CuAssertIntEquals( tc, 3, dirs.size() );
	CuAssertTrue(tc, ArchiveExistsDirectory(archive, "foo") );
	CuAssertTrue(tc, ArchiveExistsDirectory(archive, "foo/bar") );
	CuAssertTrue(tc, !ArchiveExistsDirectory(archive, "foo/spam") );
}

//-----------------------------------//

void TestArchiveZip(CuTest* tc)
{
	ArchivePtr archive( pArchiveCreateFromZip(AllocatorGetHeap(), "teste.zip") );
	CuAssertPtrNotNull(tc, archive);

	std::vector<Path> files;
	ArchiveEnumerateFiles(archive, files);
	CuAssertIntEquals( tc, 5, files.size() );

	CuAssertTrue(tc, ArchiveExistsFile(archive, "LeaksReport.txt") );
	CuAssertTrue(tc, ArchiveExistsFile(archive, "Log.html") );
	CuAssertTrue(tc, ArchiveExistsFile(archive, "foo.txt") );
	CuAssertTrue(tc, ArchiveExistsFile(archive, "bar.txt") );
	CuAssertTrue(tc, ArchiveExistsFile(archive, "files/foo.txt") );
	CuAssertTrue(tc, !ArchiveExistsFile(archive, "files/spam.txt") );

	Stream* stream = ArchiveOpenFile(archive, "file.txt", AllocatorGetHeap());
	CuAssertPtrNotNull(tc, stream);

	String text;
	StreamReadString(stream, text);
	StreamDestroy(stream);
	CuAssertStrEquals(tc, "foobar", text.c_str());

	std::vector<Path> dirs;
	ArchiveEnumerateDirectories(archive, dirs);
	
	CuAssertIntEquals( tc, 3, dirs.size() );
	CuAssertTrue(tc, ArchiveExistsDirectory(archive, "files") );
	CuAssertTrue(tc, !ArchiveExistsDirectory(archive, "dunno") );
}

//-----------------------------------//

void TestArchiveVirtual(CuTest* tc)
{
	Archive* archive_zip = ArchiveCreateFromZip(AllocatorGetHeap(), "teste.zip");
	CuAssertPtrNotNull(tc, archive_zip);

	Archive* archive_dir = ArchiveCreateFromDirectory(AllocatorGetHeap(), "teste");
	CuAssertPtrNotNull(tc, archive_dir);

	ArchivePtr archive( pArchiveCreateVirtual(AllocatorGetHeap()) );
	CuAssertPtrNotNull(tc, archive);

	ArchiveMount(archive, archive_zip, "");
	ArchiveMount(archive, archive_dir, "");

	std::vector<Path> files;
	ArchiveEnumerateFiles(archive, files);
	CuAssertIntEquals( tc, 5+4, files.size() );

	CuAssertTrue(tc, ArchiveExistsFile(archive, "foo.txt") );
	CuAssertTrue(tc, ArchiveExistsFile(archive, "bar.txt") );
	CuAssertTrue(tc, ArchiveExistsFile(archive, "files/foo.txt") );
	CuAssertTrue(tc, !ArchiveExistsFile(archive, "files/spam.txt") );
	CuAssertTrue(tc, ArchiveExistsFile(archive, "foo/bar.txt") );
	CuAssertTrue(tc, !ArchiveExistsFile(archive, "spam.txt") );

	Stream* stream = ArchiveOpenFile(archive, "file.txt", AllocatorGetHeap());
	CuAssertPtrNotNull(tc, stream);

	String text;
	StreamReadString(stream, text);
	StreamDestroy(stream);
	CuAssertStrEquals(tc, "foobar", text.c_str());

	std::vector<Path> dirs;
	ArchiveEnumerateDirectories(archive, dirs);
	
	CuAssertIntEquals( tc, 3+3, dirs.size() );
	CuAssertTrue(tc, ArchiveExistsDirectory(archive, "files") );
	CuAssertTrue(tc, !ArchiveExistsDirectory(archive, "dunno") );
	CuAssertTrue(tc, ArchiveExistsDirectory(archive, "foo") );
	CuAssertTrue(tc, ArchiveExistsDirectory(archive, "foo/bar") );
	CuAssertTrue(tc, !ArchiveExistsDirectory(archive, "foo/spam") );
}

//-----------------------------------//

CuSuite* GetSuiteArchives()
{
    CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, TestArchiveDir);
    SUITE_ADD_TEST(suite, TestArchiveZip);
	SUITE_ADD_TEST(suite, TestArchiveVirtual);
    return suite;
}