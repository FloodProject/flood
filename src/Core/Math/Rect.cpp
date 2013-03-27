/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Core/API.h"
#include "Core/Math/Rect.h"

NAMESPACE_CORE_BEGIN

//-----------------------------------//

Rect::Rect()
{
}

//-----------------------------------//

Rect::Rect(int x, int y, int width, int height)
    : x( x )
    , y( y )
    , width( width )
    , height( height )
{
}

//-----------------------------------//

bool Rect::isContainedIn(const Rect& rect) const
{
	return this->x >= rect.x && this->y >= rect.y 
		&& this->x+this->width <= rect.x+rect.width 
		&& this->y+this->height <= rect.y+rect.height;
}

//-----------------------------------//

NAMESPACE_CORE_END