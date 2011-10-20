/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Resources/API.h"

#ifdef ENABLE_FONT_BITMAP

#include "Core/Log.h"
#include "Core/Memory.h"
#include "Core/Stream.h"
#include "Core/Archive.h"
#include "Core/Utilities.h"
#include "Math/Helpers.h"

#include "Resources/FontLoader.h"
#include "Resources/ResourceManager.h"

NAMESPACE_RESOURCES_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(FontLoader, ResourceLoader)
REFLECT_CLASS_END()

//-----------------------------------//

FontLoader::FontLoader()
	: font(nullptr)
{
	extensions.push_back("font");
}

//-----------------------------------//

bool FontLoader::decode(const Stream& stream, Resource* resource)
{
	StreamReadLines((Stream*) &stream, lines);

	if( !validateFont() )
		return false;

	// TODO: ugly.
	ResourceManager* res = GetResourceManager();
	
	ResourceLoadOptions options;
	options.name = imageFilename;
	options.asynchronousLoad = false;

	image = HandleCast<Image>(res->loadResource(options));
	if( !image ) return false;

	Archive* archive = res->getArchive();

	Stream* streamGlyphs = ArchiveOpenFile(archive, glyphsFilename, AllocatorGetThis());
	if( !streamGlyphs ) return false;

	StreamRead(streamGlyphs, data);
	StreamDestroy(streamGlyphs);

	parseGlyphs();

	font = static_cast<BitmapFont*>( resource );
	font->setName( fontNameSizeInfo[0] );
	font->setSize( StringToNumber<int>(fontNameSizeInfo[1]) );
	font->setImage( image );
	font->setGlyphs( glyphs );

	return true;
}

//-----------------------------------//

void FontLoader::parseGlyphs()
{
	uint16 glyph_width = StringToNumber<uint16>( glyphSizeInfo[0] );
	uint16 glyph_height = StringToNumber<uint16>( glyphSizeInfo[1] );

	Image* pImage = image.Resolve();

	uint32 image_width = pImage->getWidth();
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

bool FontLoader::validateFont()
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

#if 0
	if( !FileExists(imageFilename) )
	{
		LogWarn( "Could not find the font image '%s'", imageFilename.c_str() );
		return false;
	}
#endif

	glyphsFilename = lines[2];

#if 0
	if( !FileExists(glyphsFilename) )
	{
		LogWarn( "Could not find the glyphs definition file '%s'", glyphsFilename.c_str() );
		return false;
	}
#endif

	StringSplit(lines[3], ' ', glyphSizeInfo);

	if( glyphSizeInfo.size() != 2 )
	{
		LogWarn( "Font description file format is invalid (glyph size)" );
		return false;
	}

	return true;
}

//-----------------------------------//

NAMESPACE_RESOURCES_END

#endif