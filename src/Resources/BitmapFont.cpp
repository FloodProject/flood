/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Resources/API.h"
#include "Resources/BitmapFont.h"

#ifdef ENABLE_FONT_BITMAP

NAMESPACE_RESOURCES_BEGIN

//-----------------------------------//

const Vector2 BitmapFont::getGlyphSize() const
{
	// TODO: ...
	return Vector2(16, 16);
}

//-----------------------------------//

NAMESPACE_RESOURCES_END

#endif