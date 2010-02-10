/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/resources/Resource.h"
#include "vapor/render/Texture.h"
#include "vapor/render/Renderable.h"

namespace vapor { namespace resources {

//-----------------------------------//

/**
 * A glyph is the information about a character/symbol in a font file.
 * This structure stores more specifically information related to its
 * position and size in the texture font file.
 */

struct Glyph
{
	ushort x;
	ushort y;
	ushort width;
	ushort height;
};

//-----------------------------------//

/**
 * A font is a sequence of characters that can be loaded as either a 
 * pre-rendered texture with the glyph information being provided as
 * a separate file, or as a TTF file that will be rendered to a
 * texture at runtime and the glyph information extracted from the 
 * tables provided by the font file. With a TTF file you are not
 * limited to a unique font size or a unique set of glyphs that were
 * pre-rendered, so you might be able to achieve better quality using
 * less resources, as the engine can create a unique texture with
 * glyphs from different fonts, and with different sizes.
 */

class VAPOR_API Font : public Resource
{
public:

	Font( const std::string& name, int size );
	virtual ~Font();

	// Gets the texture that backs this font.
	virtual render::TexturePtr getTexture() = 0;

	// Gets an array with the available glyphs information.
	virtual const std::vector<Glyph>& getGlyphsTable() const = 0;

	// Gets the size information of a glyph.
	virtual const std::pair<const ushort, const ushort> getGlyphSize() const = 0;

	// Gets the resource group of the font.
	virtual ResourceGroup::Enum getResourceGroup() const;

protected:

	// Font name.
	std::string name;
	
	// Font size.
	int size;
};

//-----------------------------------//

TYPEDEF_RESOURCE_POINTER_FROM_TYPE( Font );

//-----------------------------------//

} } // end namespaces