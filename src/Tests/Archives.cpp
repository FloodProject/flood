/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "CuTest.h"
#include <UnitTest++.h>

#include "Core/API.h"
#include "Core/Archive.h"
#include "Core/Stream.h"

//-----------------------------------//

TEST(ArchiveDir)
{
	ArchiveScopedPtr archive( ArchiveCreateFromDirectoryScoped(AllocatorGetHeap(), "teste") );
	CHECK(archive != nullptr);

	std::vector<Path> files;
	ArchiveEnumerateFiles(archive, files);
	CHECK_EQUAL(4, files.size());

	CHECK( ArchiveExistsFile(archive, "foo.txt") );
	CHECK( ArchiveExistsFile(archive, "bar.txt") );
	CHECK( ArchiveExistsFile(archive, "foo/bar.txt") );
	CHECK( !ArchiveExistsFile(archive, "spam.txt") );

	Stream* stream = ArchiveOpenFile(archive, "foo.txt", AllocatorGetHeap());
	CHECK(stream != nullptr);

	String text;
	StreamReadString(stream, text);
	StreamDestroy(stream);
	CHECK_EQUAL("foobar", text.c_str());

	std::vector<Path> dirs;
	ArchiveEnumerateDirectories(archive, dirs);
	
	CHECK_EQUAL(dirs.size(), 2);
	CHECK( ArchiveExistsDirectory(archive, "foo") );
	CHECK( ArchiveExistsDirectory(archive, "foo/bar") );
	CHECK( !ArchiveExistsDirectory(archive, "foo/spam") );
}

//-----------------------------------//

void TestArchiveZip(CuTest* tc)
{
	ArchiveScopedPtr archive( ArchiveCreateFromZipScoped(AllocatorGetHeap(), "teste.zip") );
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

	ArchiveScopedPtr archive( ArchiveCreateVirtual(AllocatorGetHeap()) );
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
	SUITE_ADD_TEST(suite, TestArchiveZip);
	SUITE_ADD_TEST(suite, TestArchiveVirtual);
	return suite;
}