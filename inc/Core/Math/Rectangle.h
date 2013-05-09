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

struct API_CORE Rectangle
{
	Rectangle();
	Rectangle(int x, int y, int width, int height);

	/// Returns true if this rectangle is contained in rect.
	bool isContainedIn(const Rectangle& rect) const;

	int x;
	int y;
	int width;
	int height;
};

//-----------------------------------//

struct API_CORE RectangleF
{
	RectangleF();
	RectangleF(float x, float y, float width, float height);

	/// Returns true if this rectangle is contained in rect.
	bool isContainedIn(const RectangleF& rect) const;

	float x;
	float y;
	float width;
	float height;
};

//-----------------------------------//

NAMESPACE_CORE_END