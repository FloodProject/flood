/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Core/API.h"
#include "Core/Archive.h"
#include "Core/Stream.h"

#include <UnitTest++.h>

SUITE(Core)
{

	TEST(ArchiveDirStream)
	{
		ArchiveScopedPtr archive( ArchiveCreateFromDirectoryScoped(AllocatorGetHeap(), "teste") );
		CHECK( archive != nullptr );

		std::vector<Path> files;
		ArchiveEnumerateFiles(archive, files);

		CHECK( ArchiveExistsFile(archive, "foo.txt") );
		CHECK( ArchiveExistsFile(archive, "bar.txt") );
		CHECK( ArchiveExistsFile(archive, "foo/bar.txt") );
		CHECK( !ArchiveExistsFile(archive, "spam.txt") );

		Stream * stream = ArchiveOpenFile(archive, "foo.txt", AllocatorGetHeap());
		CHECK( stream != nullptr);

		String text;
		stream->readString(text);
		Deallocate(stream);
		CHECK_EQUAL( "foobar", text.c_str());

		std::vector<Path> dirs;
		ArchiveEnumerateDirectories(archive, dirs);

		CHECK( dirs.size() == 2);
		CHECK( ArchiveExistsDirectory(archive, "foo"));
		CHECK( !ArchiveExistsDirectory(archive, "foo/bar"));
		CHECK( !ArchiveExistsDirectory(archive, "foo/spam"));
	}

	TEST(ArchiveZip)
	{
		ArchiveScopedPtr archive( ArchiveCreateFromZipScoped(AllocatorGetHeap(), "teste.zip"));
		CHECK( archive != nullptr );

		std::vector<Path> files;
		ArchiveEnumerateFiles(archive, files);
		CHECK( files.size() == 5 );

		CHECK( ArchiveExistsFile(archive, "LeaksReport.txt") );
		CHECK( ArchiveExistsFile(archive, "Log.html") );
		CHECK( ArchiveExistsFile(archive, "foo.txt") );
		CHECK( ArchiveExistsFile(archive, "bar.txt") );
		CHECK( ArchiveExistsFile(archive, "files/foo.txt") );
		CHECK( !ArchiveExistsFile(archive, "files/spam.txt") );

		Stream * stream = ArchiveOpenFile(archive, "file.txt", AllocatorGetHeap());
		CHECK( stream != nullptr );

		String text;
		stream->readString(text);
		Deallocate(stream);
		CHECK_EQUAL("foobar", text.c_str());

		std::vector<Path> dirs;
		ArchiveEnumerateDirectories(archive, dirs);

		CHECK( dirs.size() == 3 );
		CHECK( ArchiveExistsDirectory(archive, "files") );
		CHECK( !ArchiveExistsDirectory(archive, "dunno") );
	}

	TEST(ArchiveVirtual)
	{
		Archive* archive_zip = ArchiveCreateFromZip(AllocatorGetHeap(), "teste.zip");
		CHECK( archive_zip != nullptr );

		Archive* archive_dir = ArchiveCreateFromDirectory(AllocatorGetHeap(), "teste");
		CHECK( archive_dir != nullptr );

		ArchiveScopedPtr archive( ArchiveCreateVirtual(AllocatorGetHeap()));
		CHECK(archive != nullptr);

		ArchiveMount(archive, archive_zip, "");
		ArchiveMount(archive, archive_dir, "");

		std::vector<Path> files;
		ArchiveEnumerateFiles(archive, files);
		CHECK(files.size() == 5+4 );

		CHECK(ArchiveExistsFile(archive, "foo.txt"));
		CHECK(ArchiveExistsFile(archive, "bar.txt"));
		CHECK(ArchiveExistsFile(archive, "files/foo.txt"));
		CHECK(!ArchiveExistsFile(archive, "files/spam.txt"));
		CHECK(ArchiveExistsFile(archive, "foo/bar.txt"));
		CHECK(!ArchiveExistsFile(archive, "spam.txt"));

		Stream* stream = ArchiveOpenFile(archive, "file.txt", AllocatorGetHeap());
		CHECK(stream != nullptr);

		String text;
		stream->readString(text);
		Deallocate(stream);
		CHECK_EQUAL("foobar", text.c_str());

		std::vector<Path> dirs;
		ArchiveEnumerateDirectories(archive, dirs);

		CHECK(dirs.size() == 5);
		CHECK(ArchiveExistsDirectory(archive, "files"));
		CHECK(!ArchiveExistsDirectory(archive, "dunno"));
		CHECK(ArchiveExistsDirectory(archive, "foo"));
		CHECK(!ArchiveExistsDirectory(archive, "foo/bar"));
		CHECK(!ArchiveExistsDirectory(archive, "foo/spam"));
	}
}
