/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

#include "vapor/resources/Font_Loader.h"

#include "vapor/resources/ResourceManager.h"
#include "vapor/StringUtilities.h"
#include "vapor/Endianness.h"

#ifdef VAPOR_COMPILER_MSVC
	#pragma warning(disable : 4702 )
#endif

#include "boost/lexical_cast.hpp"

#ifdef VAPOR_COMPILER_MSVC
	#pragma warning(disable : 4702 )
#endif

using vapor::vfs::File;

//-----------------------------------//

namespace vapor {
	namespace resources {

//-----------------------------------//

Font_Loader::Font_Loader()
{
	extensions.push_back("font");
}

//-----------------------------------//

Font* Font_Loader::decode(const vfs::File& file)
{
	std::vector<std::string> lines = file.readLines();
	std::vector<std::string> info, glyphInfo;
	
	if( lines.size() == 4 )
	{
		info = split(lines[0], ' ');
		glyphInfo = split(lines[3], ' ');
	}

	// validate input
	if( lines.size() != 4 || info.size() != 2 || glyphInfo.size() != 2 )
	{
		warn( "font", "Font description file format is invalid" );
		return nullptr;
	}

	// only bitmap fonts for now
	ImagePtr img = ResourceManager::getInstancePtr()->loadResource< Image >( lines[1] );

	if( !img )
	{
		warn( "font", "Could not find the texture font file: %s", lines[1].c_str() );
		return nullptr;
	}

	if( !File::exists( lines[2] ) )
	{
		warn( "font", "Could not find the glyphs definition file: %s", lines[2].c_str() );
		return nullptr;		
	}
	
	vfs::File glyphsFile( lines[2] );

	std::vector<byte> fileData = glyphsFile.read();
	std::vector<short> data; data.resize( fileData.size() / 2 );

#ifdef VAPOR_COMPILER_MSVC
	#pragma warning( disable : 4996 )
#endif

	std::copy( &fileData.front(), &fileData.front() + fileData.size(), (char*) &data.front() );

#ifdef VAPOR_COMPILER_MSVC
	#pragma warning( default : 4996 )
#endif

	std::vector<Glyph> glyphs;

	ushort width_per_glyph = boost::lexical_cast<ushort>( glyphInfo[0] );
	ushort height_per_glyph = boost::lexical_cast<ushort>( glyphInfo[1] );

	//int n_glyphs_row = img->getWidth() / width_per_glyph;

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
	
	return new BitmapFont( info[0], boost::lexical_cast<int>(info[1]), img, glyphs );
}

//-----------------------------------//

} } // end namespaces