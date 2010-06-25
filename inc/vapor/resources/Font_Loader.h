/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/resources/ResourceLoader.h"	
#include "vapor/resources/Font.h"
#include "vapor/resources/BitmapFont.h"
#include "vapor/resources/TTF.h"

namespace vapor {

//-----------------------------------//

/**
 * Loads a font description file format that can describe both TTF and
 * bitmap fonts. The format is very simple for now. This will be changed
 * later to be based on JSON.
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

class VAPOR_API Font_Loader : public ResourceLoader
{
public:

	Font_Loader();

	// Creates the resource with no data.
	PREPARE(BitmapFont)

	// Decode a font definition.
	virtual bool decode(const File&, Resource* res);

	// Gets the name of this codec.
	GETTER(Name, const std::string, "FONTS")

	// Gets the list of extensions this codec can handle.
	GETTER(Extensions, ExtensionList&, extensions)

	// Overrides this to return the right resource group.
	GETTER(ResourceGroup, ResourceGroup::Enum, ResourceGroup::Fonts)

protected:

	// Validates that the bitmap font format is okay.
	bool validateFont();

	// Parses the glyphs definition file.
	void parseGlyphs();

	BitmapFont* font;
	ImagePtr image;
	std::vector<Glyph> glyphs;

	std::vector<std::string> fontNameSizeInfo;
	std::vector<std::string> glyphSizeInfo;
	std::string imageFilename;
	std::string glyphsFilename;

	std::vector<std::string> lines;

	std::vector<byte> data;

	// Holds all file extensions recognized by this codec.
	mutable ExtensionList extensions;
};

//-----------------------------------//

} // end namespace