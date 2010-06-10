/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/resources/Font_Loader.h"
#include "vapor/math/Math.h"

#include "vapor/resources/ResourceManager.h"
#include "vapor/Engine.h"

namespace vapor {

using vapor::File;

//-----------------------------------//

Font_Loader::Font_Loader()
{
	extensions.push_back("font");
}

//-----------------------------------//

bool Font_Loader::decode(const File& file, Resource* res)
{
	lines = file.readLines();

	if( !validateFont() )
		return false;

	// TODO: ugly.
	ResourceManager* rm = Engine::getInstance().getResourceManager();
	image = rm->loadResource<Image>( imageFilename, false );

	if( !image )
		return false;

	File glyphsFile( glyphsFilename );
	data = glyphsFile.read();

	parseGlyphs();

	font = static_cast<BitmapFont*>( res );
	font->setName( fontNameSizeInfo[0] );
	font->setSize( str_to_num<int>( fontNameSizeInfo[1] ) );
	font->setImage( image );
	font->setGlyphs( glyphs );

	return true;
}

//-----------------------------------//

void Font_Loader::parseGlyphs()
{
	ushort glyph_width = str_to_num<ushort>( glyphSizeInfo[0] );
	ushort glyph_height = str_to_num<ushort>( glyphSizeInfo[1] );

	ushort image_width = image->getWidth();
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
		warn( "font", "Font description has invalid size" );
		return false;
	}
		
	fontNameSizeInfo = String::split(lines[0], ' ');

	if( fontNameSizeInfo.size() != 2 )
	{
		warn( "font", "Font description file format is invalid (font name and size)" );
		return false;
	}

	imageFilename = lines[1];

	if( !File::exists(imageFilename) )
	{
		warn( "font", "Could not find the font image '%s'", imageFilename.c_str() );
		return false;
	}

	glyphsFilename = lines[2];

	if( !File::exists(glyphsFilename) )
	{
		warn( "font", "Could not find the glyphs definition file '%s'", glyphsFilename.c_str() );
		return false;
	}

	glyphSizeInfo = String::split(lines[3], ' ');

	if( glyphSizeInfo.size() != 2 )
	{
		warn( "font", "Font description file format is invalid (glyph size)" );
		return false;
	}

	return true;
}

//-----------------------------------//

} // end namespace