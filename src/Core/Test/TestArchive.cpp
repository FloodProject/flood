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
		ArchiveDirectory archive("teste");
		CHECK(archive.isValid);

		std::vector<Path> files;
		archive.enumerateFiles(files);

		CHECK( archive.existsFile("foo.txt") );
		CHECK( archive.existsFile("bar.txt") );
		CHECK( archive.existsFile("foo/bar.txt") );
		CHECK( !archive.existsFile("spam.txt") );

		Stream * stream = archive.openFile("foo.txt", AllocatorGetHeap());
		CHECK(stream != nullptr);

		String text;
		stream->readString(text);
		Deallocate(stream);
		CHECK_EQUAL( "foobar", text.c_str() );

		std::vector<Path> dirs;
		archive.enumerateDirs(dirs);

		CHECK( dirs.size() == 2 );
		CHECK( archive.existsDir("foo"));
		CHECK( !archive.existsDir("foo/bar") );
		CHECK( !archive.existsDir("foo/spam") );
	}

	TEST(ArchiveZip)
	{
		ArchiveZip archive("teste.zip");
		CHECK( archive.isValid );

		std::vector<Path> files;
		archive.enumerateFiles(files);
		CHECK( files.size() == 5 );

		CHECK( archive.existsFile("LeaksReport.txt") );
		CHECK( archive.existsFile("Log.html") );
		CHECK( archive.existsFile("foo.txt") );
		CHECK( archive.existsFile("bar.txt") );
		CHECK( archive.existsFile("files/foo.txt") );
		CHECK( !archive.existsFile("files/spam.txt") );

		Stream * stream = archive.openFile("file.txt", AllocatorGetHeap());
		CHECK( stream != nullptr );

		String text;
		stream->readString(text);
		Deallocate(stream);
		CHECK_EQUAL("foobar", text.c_str());

		std::vector<Path> dirs;

		archive.enumerateDirs(dirs);

		CHECK( dirs.size() == 3 );
		CHECK( archive.existsDir("files") );
		CHECK( !archive.existsDir("dunno") );
	}

	TEST(ArchiveVirtual)
	{
		Archive* archive_zip = AllocateHeap(ArchiveZip, "teste.zip");
		CHECK( archive_zip != nullptr && archive_zip->isValid);

		Archive* archive_dir = AllocateHeap(ArchiveDirectory, "teste");
		CHECK( archive_dir != nullptr && archive_dir->isValid );

		ArchiveVirtual archive;
		CHECK(archive.isValid);

		archive.mount(archive_zip, "");
		archive.mount(archive_dir, "");

		std::vector<Path> files;
		archive.enumerateFiles(files);
		CHECK(files.size() == 5+4 );

		CHECK(archive.existsFile("foo.txt"));
		CHECK(archive.existsFile("bar.txt"));
		CHECK(archive.existsFile("files/foo.txt"));
		CHECK(!archive.existsFile("files/spam.txt"));
		CHECK(archive.existsFile("foo/bar.txt"));
		CHECK(!archive.existsFile("spam.txt"));

		Stream* stream = archive.openFile("file.txt", AllocatorGetHeap());
		CHECK(stream != nullptr);

		String text;
		stream->readString(text);
		Deallocate(stream);
		CHECK_EQUAL("foobar", text.c_str());

		std::vector<Path> dirs;
		archive.enumerateDirs(dirs);
		CHECK(dirs.size() == 5);
		CHECK(archive.existsDir("files"));
		CHECK(!archive.existsDir("dunno"));
		CHECK(archive.existsDir("foo"));
		CHECK(!archive.existsDir("foo/bar"));
		CHECK(!archive.existsDir("foo/spam"));
	}
}
