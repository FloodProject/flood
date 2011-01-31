/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Resources/API.h"
#include "Resources/BitmapFont.h"

#ifdef VAPOR_FONT_BITMAP

namespace vapor {

//-----------------------------------//

const Vector2i BitmapFont::getGlyphSize() const
{
	// TODO: ...
	return Vector2i(16, 16);
}

//-----------------------------------//

} // end namespace

#endif