/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Core/API.h"
#include "Core/Pointers.h"
#include "Core/FileWatcher.h"

NAMESPACE_CORE_BEGIN

//-----------------------------------//

class Stream;
class Archive;

typedef uint32 ArchiveWatchId;
typedef Event2<Archive*, const FileWatchEvent&> ArchiveWatchEvent;

/**
 * Archives are a structured collection of files. The most common archive
 * implementations are ZIP archive files and OS filesystem directories.
 */
class API_CORE Archive
{
public:

	Archive();

	virtual ~Archive();

	/**
	 * Creates the archive.
	 * @param path path to open archive from
	 */
	Archive(const Path& path);

	/**
	 * Opens the archive.
	 * @param path path to open archive from
	 */
	virtual bool open(const Path& path) = 0;

	/**
	 * Closes the archive.
	 */
	virtual bool close() = 0;

	/**
	 * Opens a file from the archive.
	 * @param path file path
	 * @param alloc stream allocator
	 */
	virtual Stream * openFile(const Path& path, Allocator* alloc) = 0;

	/**
	 * Checks if a file exists.
	 * @param path file path
	 */
	virtual bool existsFile(const Path& path) = 0;

	/**
	 * Checks if a directory exists.
	 * @param path directory path
	 */
	virtual bool existsDir(const Path& path) = 0;;

	/**
	 * Enumerates all the files in the archive.
	 * @param paths vector to store results
	 */
	virtual void enumerateFiles(std::vector<Path>& paths) = 0;
	
	/**
	 * Enumerates all the directories in the archive.
	 * @param paths vector to store results
	 */
	virtual void enumerateDirs(std::vector<Path>& paths) = 0;

	/**
	 * Sets up and updates the watching functionality for the archive.
	 */
	virtual bool monitor() = 0;

	/**
	 * Combines the path of a file to get the full path to an archive file.
	 * @param filePath path of the file to be combined
	 */
	Path combinePath(const Path& filePath);

	const Path path; //!< archive file path
	
	void*  userdata;
	ArchiveWatchEvent watch; //!< watch event
	ArchiveWatchId watchId; //!< watch id
	bool isValid;
};

//-----------------------------------//

class API_CORE ArchiveVirtual : public Archive
{
public:

	ArchiveVirtual();

	/**
	 * @note calls @see close()
	 */
	virtual ~ArchiveVirtual();

	/**
	 * Opens the archive.
	 * @param path path to open archive from
	 */
	virtual bool open(const Path& path) override;

	/**
	 * Closes the archive.
	 */
	virtual bool close() override;

	/**
	 * Opens a file from the archive.
	 * @param path file path
	 * @param alloc stream allocator
	 */
	virtual Stream * openFile(const Path& path, Allocator* alloc) override;

	/**
	 * Checks if a file exists.
	 * @param path file path
	 */
	virtual bool existsFile(const Path& path) override;

	/**
	 * Checks if a directory exists.
	 * @param path directory path
	 */
	virtual bool existsDir(const Path& path) override;;

	/**
	 * Enumerates all the files in the archive.
	 * @param paths vector to store results
	 */
	virtual void enumerateFiles(std::vector<Path>& paths) override;
	
	/**
	 * Enumerates all the directories in the archive.
	 * @param paths vector to store results
	 */
	virtual void enumerateDirs(std::vector<Path>& paths) override;

	/**
	 * Sets up and updates the watching functionality for the archive.
	 */
	virtual bool monitor() override;

	/**
	 * Mounts an archive in the virtual archive.
	 * @param mount archive to mount
	 * @param mountPath path of archive to mount
	 */
	bool archiveMount(Archive * mount, const Path& mountPath);

	/**
	 * Mounts a directory and its direct hierarchy.
	 * @param dirPath path of directory to mount
	 * @param alloc alocator to use for mounting
	 */
	void archiveMountDirectories(const Path& dirPath, Allocator* alloc);

private:

	void enumerate(std::vector<Path>& paths, bool dir);

public:

	std::vector<Archive*> mounts; //!< mounted archives
};

//-----------------------------------//

class API_CORE ArchiveDirectory : public Archive
{
public:

	/**
	 * Creates the archive from a directory.
	 * @param path path to open archive from
	 */
	ArchiveDirectory(const Path&);

	/**
	 * @note calls @see close()
	 */
	virtual ~ArchiveDirectory();

	/**
	 * Opens the archive.
	 * @param path path to open archive from
	 */
	virtual bool open(const Path& path) override;

	/**
	 * Closes the archive.
	 */
	virtual bool close() override;

	/**
	 * Opens a file from the archive.
	 * @param path file path
	 * @param alloc stream allocator
	 */
	virtual Stream * openFile(const Path& path, Allocator* alloc) override;

	/**
	 * Checks if a file exists.
	 * @param path file path
	 */
	virtual bool existsFile(const Path& path) override;

	/**
	 * Checks if a directory exists.
	 * @param path directory path
	 */
	virtual bool existsDir(const Path& path) override;;

	/**
	 * Enumerates all the files in the archive.
	 * @param paths vector to store results
	 */
	virtual void enumerateFiles(std::vector<Path>& paths) override;
	
	/**
	 * Enumerates all the directories in the archive.
	 * @param paths vector to store results
	 */
	virtual void enumerateDirs(std::vector<Path>& paths) override;

	/**
	 * Sets up and updates the watching functionality for the archive.
	 */
	virtual bool monitor() override;
};

//-----------------------------------//

class API_CORE ArchiveZip : public Archive
{
public:

	/**
	 * Creates the archive from a ZIP.
	 * @param path path to open archive from
	 */
	ArchiveZip(const Path& path);

	/**
	 * @note calls @see close()
	 */
	virtual ~ArchiveZip();

	/**
	 * Opens the archive.
	 * @param path path to open archive from
	 */
	virtual bool open(const Path& path) override;

	/**
	 * Closes the archive.
	 */
	virtual bool close() override;

	/**
	 * Opens a file from the archive.
	 * @param path file path
	 * @param alloc stream allocator
	 */
	virtual Stream * openFile(const Path& path, Allocator* alloc) override;

	/**
	 * Checks if a file exists.
	 * @param path file path
	 */
	virtual bool existsFile(const Path& path) override;

	/**
	 * Checks if a directory exists.
	 * @param path directory path
	 */
	virtual bool existsDir(const Path& path) override;;

	/**
	 * Enumerates all the files in the archive.
	 * @param paths vector to store results
	 */
	virtual void enumerateFiles(std::vector<Path>& paths) override;
	
	/**
	 * Enumerates all the directories in the archive.
	 * @param paths vector to store results
	 */
	virtual void enumerateDirs(std::vector<Path>& paths) override;

	/**
	 * Sets up and updates the watching functionality for the archive.
	 */
	virtual bool monitor() override;

private:

	void enumerate(std::vector<Path>& paths, bool dir);

public:

	void* handle; //!< zip handle
};

//-----------------------------------//
NAMESPACE_CORE_END

