/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Resources/API.h"

#ifdef VAPOR_FONT_BITMAP

#include "Core/File.h"
#include "Math/Helpers.h"
#include "Utilities.h"

#include "Resources/Font_Loader.h"
#include "Resources/ResourceManager.h"

namespace vapor {

//-----------------------------------//

BEGIN_CLASS_PARENT(Font_Loader, ResourceLoader)
END_CLASS()

//-----------------------------------//

Font_Loader::Font_Loader()
	: font(nullptr)
{
	extensions.push_back("font");
}

//-----------------------------------//

bool Font_Loader::decode(const Stream& stream, Resource* res)
{
	lines = stream.readLines();

	if( !validateFont() )
		return false;

	// TODO: ugly.
	ResourceManager* rm = GetResourceManager();
	
	ResourceLoadOptions options;
	options.name = imageFilename;
	options.asynchronousLoading = false;

	image = RefCast<Image>( rm->loadResource(options) );

	if( !image )
		return false;

	File glyphsStream( glyphsFilename );
	glyphsStream.read(data);

	parseGlyphs();

	font = static_cast<BitmapFont*>( res );
	font->setName( fontNameSizeInfo[0] );
	font->setSize( String::toNumber<int>(fontNameSizeInfo[1]) );
	font->setImage( image );
	font->setGlyphs( glyphs );

	return true;
}

//-----------------------------------//

void Font_Loader::parseGlyphs()
{
	ushort glyph_width = String::toNumber<ushort>( glyphSizeInfo[0] );
	ushort glyph_height = String::toNumber<ushort>( glyphSizeInfo[1] );

	const uint image_width = image->getWidth();
	//ushort image_height = image->getHeight();

	glyphs.clear();

	ushort x = 0; ushort y = 0;
	for( uint i = 0; i < data.size(); i+=2 )
	{
		Glyph glyph;
		
		glyph.x = x;
		glyph.y = y;

		short* w = (short*) &data[i]; 

		glyph.width = *w;
		glyph.height = glyph_height;

		glyphs.push_back( glyph );

		if( x == image_width-glyph_width )
		{
			x = 0;
			y += glyph_height;
		} 
		else
		{
			x += glyph_width;
		}
	}
}

//-----------------------------------//

bool Font_Loader::validateFont()
{
	// Validate file length.
	if( lines.size() != 4 )
	{
		Log::warn( "Font description has invalid size" );
		return false;
	}
		
	fontNameSizeInfo = String::split(lines[0], ' ');

	if( fontNameSizeInfo.size() != 2 )
	{
		Log::warn( "Font description file format is invalid (font name and size)" );
		return false;
	}

	imageFilename = lines[1];

	if( !File::exists(imageFilename) )
	{
		Log::warn( "Could not find the font image '%s'", imageFilename.c_str() );
		return false;
	}

	glyphsFilename = lines[2];

	if( !File::exists(glyphsFilename) )
	{
		Log::warn( "Could not find the glyphs definition file '%s'", glyphsFilename.c_str() );
		return false;
	}

	glyphSizeInfo = String::split(lines[3], ' ');

	if( glyphSizeInfo.size() != 2 )
	{
		Log::warn( "Font description file format is invalid (glyph size)" );
		return false;
	}

	return true;
}

//-----------------------------------//

} // end namespace

#endif