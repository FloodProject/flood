/**
	Copyright (c) 2009 James Wynn (james@jameswynn.com)
	Copyright (c) 2010 vapor3D

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in
	all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
	THE SOFTWARE.
*/

#include "vapor/PCH.h"
#include "vapor/vfs/WatcherWin32.h"

#ifdef VAPOR_PLATFORM_WINDOWS

#ifdef VAPOR_COMPILER_MSVC
	#pragma comment(lib, "comctl32.lib")
	#pragma comment(lib, "user32.lib")
	#pragma comment(lib, "ole32.lib")

	// disable secure warnings
	#pragma warning (disable: 4996)
#endif

namespace vapor {

//-----------------------------------//

class WatcherWin32;

/// Internal watch data
struct WatchStruct
{
	OVERLAPPED mOverlapped;
	HANDLE mDirHandle;
	BYTE mBuffer[32 * 1024];
	LPARAM lParam;
	DWORD mNotifyFilter;
	bool mStopNow;
	WatcherWin32* mWatcher;
	char* mDirName;
	WatchID mWatchid;
};

//-----------------------------------//

#pragma region Internal Functions

// forward decl
bool RefreshWatch(WatchStruct* pWatch, bool _clear = false);

//-----------------------------------//

/// Unpacks events and passes them to a user defined callback.
void CALLBACK WatchCallback(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped)
{
	TCHAR szFile[MAX_PATH];
	PFILE_NOTIFY_INFORMATION pNotify;
	WatchStruct* pWatch = (WatchStruct*) lpOverlapped;
	size_t offset = 0;

	if(dwNumberOfBytesTransfered == 0)
		return;

	if (dwErrorCode == ERROR_SUCCESS)
	{
		do
		{
			pNotify = (PFILE_NOTIFY_INFORMATION) &pWatch->mBuffer[offset];
			offset += pNotify->NextEntryOffset;

			#if defined(UNICODE)
			{
				lstrcpynW(szFile, pNotify->FileName,
					std::min(MAX_PATH, static_cast<int>( pNotify->FileNameLength / sizeof(WCHAR) + 1) ));
			}
			#else
			{
				int count = WideCharToMultiByte(CP_ACP, 0, pNotify->FileName,
					pNotify->FileNameLength / sizeof(WCHAR),
					szFile, MAX_PATH - 1, nullptr, nullptr);
				szFile[count] = TEXT('\0');
			}
			#endif

			std::wstring str( szFile );
			pWatch->mWatcher->handleAction(pWatch, str, pNotify->Action);

		} while (pNotify->NextEntryOffset != 0);
	}

	if (!pWatch->mStopNow)
	{
		RefreshWatch(pWatch);
	}
}

//-----------------------------------//

/// Refreshes the directory monitoring.
bool RefreshWatch(WatchStruct* pWatch, bool _clear)
{
	return ReadDirectoryChangesW(
		pWatch->mDirHandle, pWatch->mBuffer, sizeof(pWatch->mBuffer), FALSE,
		pWatch->mNotifyFilter, nullptr, &pWatch->mOverlapped, _clear ? 0 : WatchCallback) != 0;
}

//-----------------------------------//

/// Stops monitoring a directory.
void DestroyWatch(WatchStruct* pWatch)
{
	if (pWatch)
	{
		pWatch->mStopNow = TRUE;

		CancelIo(pWatch->mDirHandle);

		RefreshWatch(pWatch, true);

		if (!HasOverlappedIoCompleted(&pWatch->mOverlapped))
		{
			SleepEx(5, TRUE);
		}

		CloseHandle(pWatch->mOverlapped.hEvent);
		CloseHandle(pWatch->mDirHandle);
		delete pWatch->mDirName;
		HeapFree(GetProcessHeap(), 0, pWatch);
	}
}

//-----------------------------------//

/// Starts monitoring a directory.
WatchStruct* CreateWatch(LPCTSTR szDirectory, DWORD mNotifyFilter)
{
	WatchStruct* pWatch;
	size_t ptrsize = sizeof(*pWatch);
	pWatch = static_cast<WatchStruct*>(HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, ptrsize));

	pWatch->mDirHandle = CreateFile(szDirectory, FILE_LIST_DIRECTORY,
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, nullptr, 
		OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED, nullptr);

	if (pWatch->mDirHandle != INVALID_HANDLE_VALUE)
	{
		pWatch->mOverlapped.hEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);
		pWatch->mNotifyFilter = mNotifyFilter;

		if (RefreshWatch(pWatch))
		{
			return pWatch;
		}
		else
		{
			CloseHandle(pWatch->mOverlapped.hEvent);
			CloseHandle(pWatch->mDirHandle);
		}
	}

	HeapFree(GetProcessHeap(), 0, pWatch);
	return nullptr;
}

#pragma endregion

//-----------------------------------//

WatcherWin32::WatcherWin32()
	: mLastWatchID(0)
{
}

//-----------------------------------//

WatcherWin32::~WatcherWin32()
{
	WatchMap::iterator iter = mWatches.begin();
	WatchMap::iterator end = mWatches.end();
	for(; iter != end; ++iter)
	{
		DestroyWatch(iter->second);
	}
	mWatches.clear();
}

//-----------------------------------//

WatchID WatcherWin32::addWatch(const std::string& directory)
{
	WatchID watchid = ++mLastWatchID;

	std::wstring wdir( directory.begin(), directory.end() );
	WatchStruct* watch = CreateWatch( wdir.c_str(),
		FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_SIZE | FILE_NOTIFY_CHANGE_FILE_NAME);

	assert( watch != nullptr );
	if(!watch) warn( "vfs", "Could not watch directory %s", directory.c_str() );

	watch->mWatchid = watchid;
	watch->mWatcher = this;
	watch->mDirName = new char[directory.length()+1];
	strcpy(watch->mDirName, directory.c_str());

	mWatches.insert(std::make_pair(watchid, watch));

	return watchid;
}

//-----------------------------------//

void WatcherWin32::removeWatch(const std::string& directory)
{
	WatchMap::iterator iter = mWatches.begin();
	WatchMap::iterator end = mWatches.end();
	for(; iter != end; ++iter)
	{
		if(directory == iter->second->mDirName)
		{
			removeWatch(iter->first);
			return;
		}
	}
}

//-----------------------------------//

void WatcherWin32::removeWatch(WatchID watchid)
{
	WatchMap::iterator iter = mWatches.find(watchid);

	if(iter == mWatches.end())
		return;

	WatchStruct* watch = iter->second;
	mWatches.erase(iter);

	DestroyWatch(watch);
}

//-----------------------------------//

void WatcherWin32::update()
{
	MsgWaitForMultipleObjectsEx(0, nullptr, 0, QS_ALLINPUT, MWMO_ALERTABLE);
}

//-----------------------------------//

void WatcherWin32::handleAction(WatchStruct* watch, const std::wstring& filename, ulong action)
{
	Action fwAction;

	switch(action)
	{
	case FILE_ACTION_ADDED:
		fwAction = Actions::Added;
		break;
	case FILE_ACTION_REMOVED:
		fwAction = Actions::Deleted;
		break;
	case FILE_ACTION_MODIFIED:
		fwAction = Actions::Modified;
		break;
	case FILE_ACTION_RENAMED_NEW_NAME:
	case FILE_ACTION_RENAMED_OLD_NAME:
		fwAction = Actions::Renamed;
		break;
	default:
		assert( 0 && "This should not be reached" );
		fwAction = Actions::Added;
	};

	// Convert wide string to regular string.
	// TODO: handle Unicode properly.
	const std::string& file = String::fromWideString(filename);

	WatchEvent we( fwAction, watch->mWatchid, watch->mDirName, file);
	onWatchEvent( we );
}

//-----------------------------------//

} // end namespace

#endif