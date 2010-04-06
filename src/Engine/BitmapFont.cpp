/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"

#include "vapor/resources/BitmapFont.h"
#include "vapor/render/TextureManager.h"

#ifdef VAPOR_FONT_BITMAP

using namespace vapor::render;

namespace vapor { namespace resources {

//-----------------------------------//

render::TexturePtr BitmapFont::getTexture()
{
	return TextureManager::getInstancePtr()->getTexture( fontImage );
}

//-----------------------------------//

const std::pair<const ushort, const ushort> BitmapFont::getGlyphSize() const
{
	// TODO: ...
	return std::make_pair( 16, 16 );
}

//-----------------------------------//

} } // end namespaces

#endif