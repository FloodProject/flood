/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Engine/API.h"
#include "Engine/Resources/Font.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

REFLECT_ABSTRACT_CHILD_CLASS(Font, Resource)
REFLECT_CLASS_END()

//-----------------------------------//

Font::Font()
	: size(0)
{ }

//-----------------------------------//

bool Font::getGlyph(int codepoint, Glyph& glyph)
{
    if (glyphs.find(codepoint) == glyphs.end())
    {
        Glyph newGlyph;
        if (!createGlyph(codepoint, newGlyph))
            return false;

        glyphs[codepoint] = newGlyph;
    }

    glyph = glyphs[codepoint];
    return true;
}

//-----------------------------------//

NAMESPACE_ENGINE_END
