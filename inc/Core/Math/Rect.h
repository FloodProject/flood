/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Core/API.h"

NAMESPACE_CORE_BEGIN

//-----------------------------------//

struct API_CORE Rect
{
	Rect();
	Rect(int x, int y, int width, int height);

	/// Returns true if this rectangle is contained in rect.
	bool isContainedIn(const Rect& rect) const;

	int x;
	int y;
	int width;
	int height;
};

//-----------------------------------//

NAMESPACE_CORE_END