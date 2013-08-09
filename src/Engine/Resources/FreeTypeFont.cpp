/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Engine/API.h"
#include "Engine/Resources/FreeTypeFont.h"

#include "Core/Math/Rectangle.h"
#include <freetype/ftglyph.h>
#include <freetype/freetype.h>
#include <freetype/ftlcdfil.h>

NAMESPACE_ENGINE_BEGIN

struct FreeTypeFont::FontInfo
{
    FT_Library  library;   /* handle to library     */
    FT_Face     face;      /* handle to face object */
};

//-----------------------------------//

FreeTypeFont::FreeTypeFont()
    :fontSize(0)
{
    fontInfo = AllocateThis(FontInfo);
}

//-----------------------------------//

FreeTypeFont::~FreeTypeFont()
{
    Deallocate(fontInfo);
}

//-----------------------------------//

void FreeTypeFont::init()
{
    int error = FT_Init_FreeType( &fontInfo->library );
    if ( error ) 
        LogError("Error initializing FreeType Library");

    error = FT_New_Memory_Face( fontInfo->library,
                              data.data(),  /* first byte in memory */
                              data.size(),  /* size in bytes        */
                              0,            /* face_index           */
                              &fontInfo->face );
  if ( error )
      LogError("Error initializing FreeType Face");
}

//-----------------------------------//

bool FreeTypeFont::getGlyphInfo(int codepoint, int fontSize, Glyph& glyph) const
{
    int glyph_index = FT_Get_Char_Index( fontInfo->face, codepoint );

    int error = FT_Set_Pixel_Sizes(
            fontInfo->face,     /* handle to face object */
            0,                  /* pixel_width           */
            fontSize );         /* pixel_height          */

    if ( error )
      LogError("Error setting font size");

    error = FT_Load_Glyph( fontInfo->face, glyph_index, FT_LOAD_RENDER );
    if ( error )
    {
        LogError("Error loading glyph");
        return false;
    }

    FT_Glyph_Metrics glyphMetrics = fontInfo->face->glyph->metrics;

    // For Some Twisted Reason, FreeType Measures Font Size
    // In Terms Of 1/64ths Of Pixels.  Thus, To Make A Font
    // h Pixels High, We Need To Request A Size Of h/64.

    glyph.width = glyphMetrics.width / 64.0;
    glyph.height = glyphMetrics.height / 64.0;
    glyph.advance = glyphMetrics.horiAdvance / 64.0;

    int baseLine = fontInfo->face->size->metrics.ascender;
    glyph.baseLineOffset = (baseLine - glyphMetrics.horiBearingY) / 64.0;
    glyph.xOffset = glyphMetrics.horiBearingX / 64.0;

    return true;
}

ImageHandle FreeTypeFont::createGlyphImage(int codepoint, int fontSize) const
{
    int glyph_index = FT_Get_Char_Index( fontInfo->face, codepoint );

    int error = FT_Set_Pixel_Sizes(
            fontInfo->face,     /* handle to face object */
            0,                  /* pixel_width           */
            fontSize );         /* pixel_height          */

    if ( error )
      LogError("Error setting font size");

    error = FT_Load_Glyph( fontInfo->face, glyph_index, FT_LOAD_RENDER | FT_LOAD_TARGET_LCD);
    if ( error ) 
        LogError("Error loading glyph");

    FT_Glyph  glyph; /* a handle to the glyph image */    

    error = FT_Get_Glyph( fontInfo->face->glyph, &glyph );
    if ( error ) 
        LogError("Error getting glyph");

    FT_Vector  origin;
    origin.x = origin.y = 0;

    error = FT_Glyph_To_Bitmap(
            &glyph,
            FT_RENDER_MODE_LCD,
            &origin,
            1 );

    FT_Bitmap bitmap = ((FT_BitmapGlyph) glyph)->bitmap;

    if (bitmap.width == 0 || bitmap.rows == 0)
        return HandleInvalid;

    auto image = ImageCreate(AllocatorGetHeap(), bitmap.width/3, bitmap.rows, PixelFormat::R8G8B8);
    image.Resolve()->setBuffer(bitmap.buffer, bitmap.pitch);
    return image;
}

Vector2 FreeTypeFont::getKerning(int codepoint1, int codepoint2, int fontSize) const
{
    int error = FT_Set_Pixel_Sizes(
            fontInfo->face,     /* handle to face object */
            0,                  /* pixel_width           */
            fontSize );         /* pixel_height          */

    if ( error )
        LogError("Error setting font size");

    FT_Vector  kerning;

    int left = FT_Get_Char_Index( fontInfo->face, codepoint1 );
    int right = FT_Get_Char_Index( fontInfo->face, codepoint2 );

    error = FT_Get_Kerning( fontInfo->face,           /* handle to face object */    
                            left,                     /* left glyph index      */    
                            right,                    /* right glyph index     */    
                            FT_KERNING_DEFAULT,       /* kerning mode          */    
                            &kerning );               /* target vector         */

    if ( error )
        LogError("Error getting font kerning");

    return Vector2(kerning.x / 64.0, kerning.y / 64.0);
}

NAMESPACE_ENGINE_END
