/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Core/API.h"
#include "Core/Math/Rectangle.h"

NAMESPACE_CORE_BEGIN

//-----------------------------------//

Rectangle::Rectangle()
{
}

//-----------------------------------//

Rectangle::Rectangle(int x, int y, int width, int height)
    : x( x )
    , y( y )
    , width( width )
    , height( height )
{
}

//-----------------------------------//

bool Rectangle::isContainedIn(const Rectangle& rect) const
{
    return this->x >= rect.x && this->y >= rect.y 
        && this->x+this->width <= rect.x+rect.width 
        && this->y+this->height <= rect.y+rect.height;
}

//-----------------------------------//

RectangleF::RectangleF()
{
}

//-----------------------------------//

RectangleF::RectangleF(float x, float y, float width, float height)
    : x( x )
    , y( y )
    , width( width )
    , height( height )
{
}

//-----------------------------------//

bool RectangleF::isContainedIn(const RectangleF& rect) const
{
    return this->x >= rect.x && this->y >= rect.y 
        && this->x+this->width <= rect.x+rect.width 
        && this->y+this->height <= rect.y+rect.height;
}

//-----------------------------------//

NAMESPACE_CORE_END