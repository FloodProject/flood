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

BitmapFont::BitmapFont( const std::string& font, int size, ImagePtr img, 
					   const std::vector<Glyph>& glyphs )
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

const std::vector<Glyph>& BitmapFont::getGlyphsTable() const
{
	return glyphData;
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