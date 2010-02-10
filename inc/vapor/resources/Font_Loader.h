/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/CompileOptions.h"

#include "vapor/resources/ResourceLoader.h"	

#include "vapor/resources/Font.h"

#ifdef VAPOR_FONT_BITMAP
	#include "vapor/resources/BitmapFont.h"
#endif

#ifdef VAPOR_FONT_FREETYPE2
	#include "vapor/resources/TTF.h"
#endif

namespace vapor { namespace resources {

/**
 * Loads a font description file format that can describe both TTF and
 * bitmap fonts. The format is very simple for now, it only supports two
 * lines of text. The first should be the name of the texture font file
 * and the second the glyph definition format. This will be changed later
 * to be based on JSON.
 *
 * Example:
 *
 *		Calibri 12
 *		calibri-12.png
 *		calibri-12.dat
 *		16 16
 *
 * TODO:
 *
 *		"font": {
 *          "type": "texture"
 *			"name" : "Calibri",
 *			"size": 12,
 *			"glyphs": {
 *				"a": { "codepoint": 0x10, "uv": [[0, 0], [10, 20]] }
 *				"b": { "codepoint": 0x11, "uv": [[0, 0], [10, 20]] }
 *				"c": { "codepoint": 0x12, "uv": [[0, 0], [10, 20]] }
 *			}
 *		}   
 */

class Font_Loader : public ResourceLoader
{
public:

	Font_Loader();

	// Decode a font definition.
	virtual Font* decode(const vfs::File& file);

	// Gets the name of this codec.
	virtual const std::string getName() { return "FONTS"; }

	// Gets the list of extensions this codec can handle.
	virtual 
		std::list< std::string >& getExtensions() { return extensions; }

	// Overrides this to return the right resource group.
	virtual ResourceGroup::Enum getResourceGroup() { return ResourceGroup::Fonts; }

protected:

	// Holds all file extensions recognized by this codec.
	std::list< std::string > extensions;
};

} } // end namespaces