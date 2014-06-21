/************************************************************************
*
* Flood Project � (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Core/API.h"
#include "Core/Archive.h"
#include "Core/Stream.h"
#include "Core/Memory.h"
#include "Core/Log.h"

#ifdef ENABLE_ARCHIVE_VIRTUAL

NAMESPACE_CORE_BEGIN

//-----------------------------------//

ArchiveVirtual::ArchiveVirtual() 
    : Archive("")
{
}

//-----------------------------------//

ArchiveVirtual::~ArchiveVirtual()
{
    close();
}

//-----------------------------------//

static void HandleWatch(Archive*, const FileWatchEvent& event);

bool ArchiveVirtual::mount(Archive * mount, const Path& mountPath)
{
    mounts.Push(mount);

    /// Setup archive watch callbacks.
    mount->userdata = this;
    mount->watch.Connect(HandleWatch);

    return true;
}

//-----------------------------------//

void ArchiveVirtual::mountDirectories(const Path& dirPath, Allocator* alloc)
{
    Archive* dir = Allocate(alloc, ArchiveDirectory, dirPath);
    if (!dir) return;

    mount(dir, "");
    
    Vector<Path> dirs;
    dir->enumerateDirs(dirs);

    for(auto& dir : dirs)
    {
        auto& path = PathCombine(dirPath, dir);
        Archive* ndir = Allocate(alloc, ArchiveDirectory, path);
        mount(ndir, "");
    }
}

//-----------------------------------//

bool ArchiveVirtual::open(const Path& path)
{
    isValid = true; 
    return isValid;
}

//-----------------------------------//

bool ArchiveVirtual::close()
{
    for(auto& i : mounts)
    {
        Archive* marchive = i;
        Deallocate(marchive);
    }

    mounts.Clear();

    return true;
}

//-----------------------------------//

Stream* ArchiveVirtual::openFile(const Path& path, Allocator* alloc)
{
    if (mounts.Empty()) return nullptr;

    Stream* stream = nullptr;

    for(auto& i : mounts)
    {
        Archive* marchive = i;
        stream = marchive->openFile(path, alloc);
        if (stream) break;
    }

    return stream;
}

//-----------------------------------//

void ArchiveVirtual::enumerate(Vector<Path>& paths, bool dir)
{

    for(auto& i : mounts)
    {
        Archive* marchive = i;

        if (dir) marchive->enumerateDirs(paths);
        else marchive->enumerateFiles(paths);
    }
}

//-----------------------------------//

void ArchiveVirtual::enumerateFiles(Vector<Path>& paths)
{
    enumerate(paths, false);
}

//-----------------------------------//

void ArchiveVirtual::enumerateDirs(Vector<Path>& paths)
{
    enumerate(paths, true);
}

//-----------------------------------//

bool ArchiveVirtual::existsFile(const Path& path)
{
    for(auto& i : mounts)
        if (i->existsFile(path))
            return true;

    return false;
}

//-----------------------------------//

bool ArchiveVirtual::existsDir(const Path& path)
{
    for(auto& i : mounts)
        if(i->existsDir(path))
            return true;

    return false;
}

//-----------------------------------//

static void HandleWatch(Archive* archive, const FileWatchEvent& event)
{
    Archive* varchive = (ArchiveVirtual*) archive->userdata;
    varchive->watch(archive, event);
}

bool ArchiveVirtual::monitor()
{
    for(auto& i : mounts)
        i->monitor();

    return true;
}

//-----------------------------------//

NAMESPACE_CORE_END

#endif
