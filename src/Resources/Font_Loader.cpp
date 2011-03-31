/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Resources/API.h"

#ifdef VAPOR_FONT_BITMAP

#include "Core/Log.h"
#include "Core/Memory.h"
#include "Core/Stream.h"
#include "Core/Archive.h"
#include "Core/Utilities.h"
#include "Math/Helpers.h"

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
	StreamReadLines((Stream*) &stream, lines);

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

	Stream* streamGlyphs = StreamCreateFromFile(
		AllocatorGetDefault(), glyphsFilename, StreamMode::Read);
	
	StreamRead(streamGlyphs, data);
	StreamDestroy(streamGlyphs, AllocatorGetDefault());

	parseGlyphs();

	font = static_cast<BitmapFont*>( res );
	font->setName( fontNameSizeInfo[0] );
	font->setSize( StringToNumber<int>(fontNameSizeInfo[1]) );
	font->setImage( image );
	font->setGlyphs( glyphs );

	return true;
}

//-----------------------------------//

void Font_Loader::parseGlyphs()
{
	uint16 glyph_width = StringToNumber<uint16>( glyphSizeInfo[0] );
	uint16 glyph_height = StringToNumber<uint16>( glyphSizeInfo[1] );

	uint32 image_width = image->getWidth();
	//ushort image_height = image->getHeight();

	glyphs.clear();

	uint16 x = 0;
	uint16 y = 0;
	
	for( size_t i = 0; i < data.size(); i+=2 )
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
		LogWarn( "Font description has invalid size" );
		return false;
	}
		
	StringSplit(lines[0], ' ', fontNameSizeInfo);

	if( fontNameSizeInfo.size() != 2 )
	{
		LogWarn( "Font description file format is invalid (font name and size)" );
		return false;
	}

	imageFilename = lines[1];

	if( !FileExists(imageFilename) )
	{
		LogWarn( "Could not find the font image '%s'", imageFilename.c_str() );
		return false;
	}

	glyphsFilename = lines[2];

	if( !FileExists(glyphsFilename) )
	{
		LogWarn( "Could not find the glyphs definition file '%s'", glyphsFilename.c_str() );
		return false;
	}

	StringSplit(lines[3], ' ', glyphSizeInfo);

	if( glyphSizeInfo.size() != 2 )
	{
		LogWarn( "Font description file format is invalid (glyph size)" );
		return false;
	}

	return true;
}

//-----------------------------------//

} // end namespace

#endif