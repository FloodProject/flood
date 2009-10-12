/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

#include "vapor/resources/BitmapFont.h"
#include "vapor/render/TextureManager.h"

#ifdef VAPOR_FONT_BITMAP

using namespace vapor::render;

namespace vapor {
	namespace resources {

//-----------------------------------//

BitmapFont::BitmapFont( const std::string& font, int size, ImagePtr img, 
					   std::vector<Glyph> glyphs )
	: Font( font, size ), fontImage( img ), glyphData( glyphs )
{
	
}

//-----------------------------------//

BitmapFont::~BitmapFont()
{

}

//-----------------------------------//

render::TexturePtr BitmapFont::getTexture()
{
	return TextureManager::getInstancePtr()->getTexture( fontImage );
}

//-----------------------------------//

const std::vector<Glyph>& BitmapFont::getGlyphInformation() const
{
	return glyphData;
}

//-----------------------------------//

} } // end namespaces

#endif