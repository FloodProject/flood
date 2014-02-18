/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "CppSharp.h"
#include <Core/Archive.h>

namespace Flood
{
    ref class Allocator;
    ref class Archive;
    ref class ArchiveDirectory;
    ref class ArchiveVirtual;
    ref class ArchiveZip;
    ref class Stream;
    value struct FileWatchEvent;
}

namespace Flood
{
    /// <summary>
    /// Archives are a structured collection of files. The most common archive
    /// implementations are ZIP archive files and OS filesystem directories.
    /// </summary>
    public ref class Archive : ICppInstance
    {
    public:

        property ::Archive* NativePtr;
        property System::IntPtr __Instance
        {
            virtual System::IntPtr get();
            virtual void set(System::IntPtr instance);
        }

        Archive(::Archive* native);
        Archive(System::IntPtr native);
        Archive();

        /// <summary>
        /// Creates the archive.
        /// </summary>
        Archive(System::String^ path);

        property System::String^ Path
        {
            System::String^ get();
        }

        property System::IntPtr Userdata
        {
            System::IntPtr get();
            void set(System::IntPtr);
        }

        property unsigned int WatchId
        {
            unsigned int get();
            void set(unsigned int);
        }

        property bool IsValid
        {
            bool get();
            void set(bool);
        }

        /// <summary>
        /// Opens the archive.
        /// </summary>
        virtual bool Open(System::String^ path);

        /// <summary>
        /// Closes the archive.
        /// </summary>
        virtual bool Close();

        /// <summary>
        /// Opens a file from the archive.
        /// </summary>
        virtual Flood::Stream^ OpenFile(System::String^ path, Flood::Allocator^ alloc);

        /// <summary>
        /// Checks if a file exists.
        /// </summary>
        virtual bool ExistsFile(System::String^ path);

        /// <summary>
        /// Checks if a directory exists.
        /// </summary>
        virtual bool ExistsDir(System::String^ path);

        /// <summary>
        /// Enumerates all the files in the archive.
        /// </summary>
        virtual void EnumerateFiles(System::Collections::Generic::List<System::String^>^ paths);

        /// <summary>
        /// Enumerates all the directories in the archive.
        /// </summary>
        virtual void EnumerateDirs(System::Collections::Generic::List<System::String^>^ paths);

        /// <summary>
        /// Sets up and updates the watching functionality for the archive.
        /// </summary>
        virtual bool Monitor();

        /// <summary>
        /// Combines the path of a file to get the full path to an archive file.
        /// </summary>
        System::String^ CombinePath(System::String^ filePath);

        virtual bool Equals(System::Object^ object) override;

        virtual int GetHashCode() override;
    };

    public ref class ArchiveVirtual : Flood::Archive
    {
    public:

        ArchiveVirtual(::ArchiveVirtual* native);
        ArchiveVirtual(System::IntPtr native);
        ArchiveVirtual();

        property System::Collections::Generic::List<Flood::Archive^>^ Mounts
        {
            System::Collections::Generic::List<Flood::Archive^>^ get();
            void set(System::Collections::Generic::List<Flood::Archive^>^);
        }

        /// <summary>
        /// Opens the archive.
        /// </summary>
        virtual bool Open(System::String^ path) override;

        /// <summary>
        /// Closes the archive.
        /// </summary>
        virtual bool Close() override;

        /// <summary>
        /// Opens a file from the archive.
        /// </summary>
        virtual Flood::Stream^ OpenFile(System::String^ path, Flood::Allocator^ alloc) override;

        /// <summary>
        /// Checks if a file exists.
        /// </summary>
        virtual bool ExistsFile(System::String^ path) override;

        /// <summary>
        /// Checks if a directory exists.
        /// </summary>
        virtual bool ExistsDir(System::String^ path) override;

        /// <summary>
        /// Enumerates all the files in the archive.
        /// </summary>
        virtual void EnumerateFiles(System::Collections::Generic::List<System::String^>^ paths) override;

        /// <summary>
        /// Enumerates all the directories in the archive.
        /// </summary>
        virtual void EnumerateDirs(System::Collections::Generic::List<System::String^>^ paths) override;

        /// <summary>
        /// Sets up and updates the watching functionality for the archive.
        /// </summary>
        virtual bool Monitor() override;

        /// <summary>
        /// Mounts an archive in the virtual archive.
        /// </summary>
        bool Mount(Flood::Archive^ mount, System::String^ mountPath);

        /// <summary>
        /// Mounts a directory and its direct hierarchy.
        /// </summary>
        void MountDirectories(System::String^ dirPath, Flood::Allocator^ alloc);

        virtual bool Equals(System::Object^ object) override;

        virtual int GetHashCode() override;
    };

    public ref class ArchiveDirectory : Flood::Archive
    {
    public:

        ArchiveDirectory(::ArchiveDirectory* native);
        ArchiveDirectory(System::IntPtr native);
        /// <summary>
        /// Creates the archive from a directory.
        /// </summary>
        ArchiveDirectory(System::String^ _0);

        /// <summary>
        /// Opens the archive.
        /// </summary>
        virtual bool Open(System::String^ path) override;

        /// <summary>
        /// Closes the archive.
        /// </summary>
        virtual bool Close() override;

        /// <summary>
        /// Opens a file from the archive.
        /// </summary>
        virtual Flood::Stream^ OpenFile(System::String^ path, Flood::Allocator^ alloc) override;

        /// <summary>
        /// Checks if a file exists.
        /// </summary>
        virtual bool ExistsFile(System::String^ path) override;

        /// <summary>
        /// Checks if a directory exists.
        /// </summary>
        virtual bool ExistsDir(System::String^ path) override;

        /// <summary>
        /// Enumerates all the files in the archive.
        /// </summary>
        virtual void EnumerateFiles(System::Collections::Generic::List<System::String^>^ paths) override;

        /// <summary>
        /// Enumerates all the directories in the archive.
        /// </summary>
        virtual void EnumerateDirs(System::Collections::Generic::List<System::String^>^ paths) override;

        /// <summary>
        /// Sets up and updates the watching functionality for the archive.
        /// </summary>
        virtual bool Monitor() override;

        virtual bool Equals(System::Object^ object) override;

        virtual int GetHashCode() override;
    };

    public ref class ArchiveZip : Flood::Archive
    {
    public:

        ArchiveZip(::ArchiveZip* native);
        ArchiveZip(System::IntPtr native);
        /// <summary>
        /// Creates the archive from a ZIP.
        /// </summary>
        ArchiveZip(System::String^ path);

        property System::IntPtr Handle
        {
            System::IntPtr get();
            void set(System::IntPtr);
        }

        /// <summary>
        /// Opens the archive.
        /// </summary>
        virtual bool Open(System::String^ path) override;

        /// <summary>
        /// Closes the archive.
        /// </summary>
        virtual bool Close() override;

        /// <summary>
        /// Opens a file from the archive.
        /// </summary>
        virtual Flood::Stream^ OpenFile(System::String^ path, Flood::Allocator^ alloc) override;

        /// <summary>
        /// Checks if a file exists.
        /// </summary>
        virtual bool ExistsFile(System::String^ path) override;

        /// <summary>
        /// Checks if a directory exists.
        /// </summary>
        virtual bool ExistsDir(System::String^ path) override;

        /// <summary>
        /// Enumerates all the files in the archive.
        /// </summary>
        virtual void EnumerateFiles(System::Collections::Generic::List<System::String^>^ paths) override;

        /// <summary>
        /// Enumerates all the directories in the archive.
        /// </summary>
        virtual void EnumerateDirs(System::Collections::Generic::List<System::String^>^ paths) override;

        /// <summary>
        /// Sets up and updates the watching functionality for the archive.
        /// </summary>
        virtual bool Monitor() override;

        virtual bool Equals(System::Object^ object) override;

        virtual int GetHashCode() override;
    };
}
