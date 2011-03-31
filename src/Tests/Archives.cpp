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
#include "Core/Memory.h"
#include "Core/Pointers.h"

using namespace vapor;

void TestArchive(CuTest *tc)
{
	Archive* archive = ArchiveCreateFromZip( AllocatorGetDefault(), "teste.zip" );
	
	std::vector<Path> files;
	ArchiveEnumerateFiles(archive, files);
	CuAssertIntEquals( tc, 4, files.size() );

	CuAssertTrue(tc, ArchiveExistsFile(archive, "LeaksReport.txt") );
	CuAssertTrue(tc, ArchiveExistsFile(archive, "Log.html") );
	CuAssertTrue(tc, ArchiveExistsFile(archive, "foo.txt") );
	CuAssertTrue(tc, ArchiveExistsFile(archive, "bar.txt") );

	std::vector<Path> dirs;
	ArchiveEnumerateDirectories(archive, dirs);
	CuAssertIntEquals( tc, 3, dirs.size() );
	CuAssertTrue(tc, ArchiveExistsDirectory(archive, "files") );

	ArchiveDestroy( archive, AllocatorGetDefault() );
}

CuSuite* GetSuiteArchives()
{
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, TestArchive);
    return suite;
}