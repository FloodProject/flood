/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Engine/API.h"
#include "Engine/Resources/BitmapFont.h"

#ifdef ENABLE_FONT_BITMAP

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

bool BitmapFont::createGlyph(int codepoint, Glyph& glyph) const
{
	// TODO Read glyph data
    return false;
}

//-----------------------------------//

Vector2i BitmapFont::getKerning(int codepoint1, int codepoint2)  const 
{
	// TODO
    return Vector2i();
}

NAMESPACE_ENGINE_END

#endif