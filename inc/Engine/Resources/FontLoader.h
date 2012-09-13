/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#ifdef ENABLE_FONT_BITMAP

#include "Resources/ResourceLoader.h"	
#include "Engine/Resources/Font.h"
#include "Engine/Resources/BitmapFont.h"
#include "Engine/Resources/TTF.h"

NAMESPACE_ENGINE_BEGIN

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

REFLECT_DECLARE_CLASS(FontLoader)

class API_ENGINE FontLoader : public ResourceLoader
{
	REFLECT_DECLARE_OBJECT(FontLoader)

public:

	FontLoader();

	// Creates the resource with no data.
	RESOURCE_LOADER_PREPARE(BitmapFont)

	// Gets the class of the resource.
	RESOURCE_LOADER_CLASS(Font)

	// Decode a font definition.
	virtual bool decode(ResourceLoadOptions&) OVERRIDE;

	// Gets the name of this codec.
	GETTER(Name, const String, "FONTS")

	// Overrides this to return the right resource group.
	GETTER(ResourceGroup, ResourceGroup, ResourceGroup::Fonts)

protected:

	// Validates that the bitmap font format is okay.
	bool validateFont();

	// Parses the glyphs definition file.
	void parseGlyphs();

	BitmapFont* font;
	ImageHandle image;
	std::vector<Glyph> glyphs;

	std::vector<String> fontNameSizeInfo;
	std::vector<String> glyphSizeInfo;
	String imageFilename;
	String glyphsFilename;

	std::vector<String> lines;
	std::vector<byte> data;
};

//-----------------------------------//

NAMESPACE_ENGINE_END

#endif