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

const Vector2 BitmapFont::getGlyphSize() const
{
	// TODO: ...
	return Vector2(16, 16);
}

//-----------------------------------//

NAMESPACE_ENGINE_END

#endif