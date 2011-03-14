/**
	Implementation for Windows. Uses ReadDirectoryChangesW to watch for
	file system changes.

	@author James Wynn
	@date 4/15/2009

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

#pragma once

#ifdef VAPOR_PLATFORM_WINDOWS

#include "Core/FileWatcher.h"

namespace vapor {

//-----------------------------------//

struct WatchStruct;

/// type for a map from WatchID to WatchStruct pointer
typedef std::map<WatchID, WatchStruct*> WatchMap;

//-----------------------------------//

/// Implementation for Win32 based on ReadDirectoryChangesW.
/// @class FileWatcherWin32
class FileWatcherWin32 : public FileWatcher
{
public:

	FileWatcherWin32();
	virtual ~FileWatcherWin32();

	/// Add a directory watch
	/// @exception FileNotFoundException Thrown when the requested directory does not exist
	WatchID addWatch(const String& directory);

	/// Remove a directory watch. This is a brute force lazy search O(nlogn).
	void removeWatch(const String& directory);

	/// Remove a directory watch. This is a map lookup O(logn).
	void removeWatch(WatchID watchid);

	/// Updates the watcher. Must be called often.
	void update();

	/// Handles the action
	void handleAction(WatchStruct* watch, const std::wstring& filename, uint32 action);

private:

	/// Map of WatchID to WatchStruct pointers
	WatchMap mWatches;
	
	/// The last watchid
	WatchID mLastWatchID;
};

//-----------------------------------//

} // end namespace

#endif
