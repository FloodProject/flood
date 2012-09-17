/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Core/API.h"
#include "Core/Log.h"
#include "Core/Memory.h"

#ifdef PLATFORM_WINDOWS
	#include "Core/FileWatcherWin32.h"
#endif

#ifdef API_CORE_DLL_EXPORT
FL_INSTANTIATE_TEMPLATES()
#endif

NAMESPACE_CORE_BEGIN

static FileWatcher* gs_FileWatcher = nullptr;

FileWatcher* GetFileWatcher()
{
	assert(gs_FileWatcher && "Expected a valid instance");
	return gs_FileWatcher;
}

static void InitializeFileWatcher()
{
	LogInfo("Creating the file watcher");

#ifdef PLATFORM_WINDOWS
	gs_FileWatcher = AllocateHeap(FileWatcherWin32);
#endif
}

void DeinitializeFileWatcher()
{
	Deallocate(gs_FileWatcher);
}

void CoreInitialize()
{
	InitializeFileWatcher();
}

void CoreDeinitialize()
{
	DeinitializeFileWatcher();
}

NAMESPACE_CORE_END