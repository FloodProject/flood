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
#include "vapor/vfs/Watcher.h"

namespace vapor {

//-----------------------------------//

const std::string Actions::getString( Actions::Enum action )
{
	switch(action)
	{
	case Actions::Added:
		return "Added";
	case Actions::Deleted:
		return "Deleted";
	case Actions::Modified:
		return "Modified";
	case Actions::Renamed:
		return "Renamed";
	default:
		return "(unknown)";
	}
}

//-----------------------------------//

} // end namespace
