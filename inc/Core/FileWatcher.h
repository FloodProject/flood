/**
	Main header for the FileWatcher class. Declares all implementation
	classes to reduce compilation overhead.

	@author James Wynn
	@date 4/15/2009

	Copyright (c) 2009 James Wynn (james@jameswynn.com)
	Copyright (c) 2010 João Matos (triton@vapor3d.org)

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

#include "Core/Event.h"

NAMESPACE_CORE_BEGIN

//-----------------------------------//

/**
 * This data will be sent when a file notification is sent from the
 * OS back to the engine. You can hook the delegate in the FileWatcher
 * class to get these events.
 */

typedef uint32 FileWatchId;

struct API_CORE FileWatchEvent
{
	DECLARE_UNCOPYABLE(FileWatchEvent)

	enum Enum
	{
		Added,
		Deleted,
		Modified,
		Renamed,
	};

	FileWatchEvent( FileWatchEvent::Enum, FileWatchId,
		const String& dir, const String& file );

	FileWatchEvent::Enum action;
	FileWatchId watchId;

	String dir;
	String filename;
	void* userdata;
};

//-----------------------------------//

/** 
 * Listens to files and directories and dispatches events
 * to notify the parent program of the changes.
 */

class API_CORE NO_VTABLE FileWatcher
{
	DECLARE_UNCOPYABLE(FileWatcher)

public:

	FileWatcher() {}
	virtual ~FileWatcher() {}

	// Add a directory watch
	virtual FileWatchId addWatch(const String& directory, void* userdata = 0) = 0;

	// Remove a directory watch. This is a brute force search O(nlogn).
	virtual void removeWatch(const String& directory) = 0;

	// Remove a directory watch. This is a map lookup O(logn).
	virtual void removeWatch(FileWatchId FileWatchId) = 0;

	// Updates the watcher. Must be called often.
	virtual void update() = 0;

	// Fired up when the watcher gets notified by the OS.
	Event1<const FileWatchEvent&> onFileWatchEvent;
};

//-----------------------------------//

NAMESPACE_CORE_END