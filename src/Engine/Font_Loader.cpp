/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/resources/Font_Loader.h"

using vapor::vfs::File;

namespace vapor { namespace resources {

//-----------------------------------//

Font_Loader::Font_Loader()
{
	extensions.push_back("font");
}

//-----------------------------------//

bool Font_Loader::decode(const vfs::File& file, Resource* res)
{
	std::vector<std::string> lines = file.readLines();
	std::vector<std::string> info, glyphInfo;
	
	if( lines.size() == 4 )
	{
		info = String::split(lines[0], ' ');
		glyphInfo = String::split(lines[3], ' ');
	}

	// validate input
	if( lines.size() != 4 || info.size() != 2 || glyphInfo.size() != 2 )
	{
		warn( "font", "Font description file format is invalid" );
		return false;
	}

	// only bitmap fonts for now
	//ImagePtr img = Engine::getResourceManager()->loadResource< Image >( lines[1] );
	ImagePtr img;

	if( !img )
	{
		warn( "font", "Could not find the texture font file: %s", lines[1].c_str() );
		return false;
	}

	if( !File::exists( lines[2] ) )
	{
		warn( "font", "Could not find the glyphs definition file: %s", lines[2].c_str() );
		return false;		
	}
	
	File glyphsFile( lines[2] );

	std::vector<byte> fileData = glyphsFile.read();
	std::vector<short> data; data.resize( fileData.size() / 2 );
	std::copy( &fileData.front(), &fileData.front() + fileData.size(), (char*) &data.front() );
	
	ushort width_per_glyph = str_to_num<ushort>( glyphInfo[0] );
	ushort height_per_glyph = str_to_num<ushort>( glyphInfo[1] );

	std::vector<Glyph> glyphs;

	ushort x = 0; ushort y = 0;
	for( uint i = 0; i < data.size(); i++ )
	{
		Glyph glyph;
		
		glyph.x = x;
		glyph.y = y;

		glyph.width = data[i];
		glyph.height = height_per_glyph;

		glyphs.push_back( glyph );

		if( x == img->getWidth()-width_per_glyph )
		{
			x = 0;
			y += height_per_glyph;
		} 
		else
		{
			x += width_per_glyph;
		}
	}
	
	BitmapFont* font = static_cast<BitmapFont*>( res );
	font->setName( info[0] );
	font->setSize( str_to_num<int>(info[1]) );
	font->setImage( img );
	font->setGlyphs( glyphs );
	
	return true;
}

//-----------------------------------//

} } // end namespaces