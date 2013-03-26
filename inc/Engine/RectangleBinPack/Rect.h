/** @file Rect.h
	@author Jukka Jylänki

	This work is released to Public Domain, do whatever you want with it.
*/
#pragma once

#include "Engine/API.h"

struct RectSize
{
	int width;
	int height;
};

struct Rect
{
	int x;
	int y;
	int width;
	int height;
};

/// Returns true if a is contained in b.
bool API_ENGINE IsContainedIn(const Rect &a, const Rect &b);
