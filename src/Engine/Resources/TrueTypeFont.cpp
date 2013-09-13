/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Engine/API.h"
#include "Engine/Resources/TrueTypeFont.h"

#include "Core/Math/Rectangle.h"
#include <stb_truetype.h>

NAMESPACE_ENGINE_BEGIN

struct TrueTypeFont::FontInfo
{
    stbtt_fontinfo font;
};

//-----------------------------------//

TrueTypeFont::TrueTypeFont()
    : data(*AllocatorGetHeap())
{
    fontInfo = AllocateThis(FontInfo);
}

//-----------------------------------//

TrueTypeFont::~TrueTypeFont()
{
    Deallocate(fontInfo);
}

//-----------------------------------//

void TrueTypeFont::init()
{
    stbtt_InitFont(&fontInfo->font, data.data(), 0);
}

//-----------------------------------//

bool TrueTypeFont::getGlyphInfo(int codepoint, int fontSize, Glyph& glyph) const
{
    float scale = stbtt_ScaleForPixelHeight(&fontInfo->font,(float) fontSize);

    int ascent;
    stbtt_GetFontVMetrics(&fontInfo->font, &ascent,0,0);
    float baseLine = ascent * scale;

    int x0,x1,y0,y1;
    stbtt_GetCodepointBitmapBoxSubpixel(&fontInfo->font, codepoint, scale,scale,0,0, &x0,&y0,&x1,&y1);
    glyph.baseLineOffset = baseLine + y0;

    int advance;
    stbtt_GetCodepointHMetrics(&fontInfo->font, codepoint, &advance, 0);
    glyph.advance = advance*scale;

    glyph.width = x1-x0;
    glyph.height = y1-y0;

    return true;
}

ImageHandle TrueTypeFont::createGlyphImage(int codepoint, int fontSize) const
{
    float scale = stbtt_ScaleForPixelHeight(&fontInfo->font,(float) fontSize);

    int width,height;
    byte* pixelBuffer;

    pixelBuffer = stbtt_GetCodepointBitmap(&fontInfo->font, 0, scale, codepoint, &width, &height, 0,0);

    if (width == 0 || height == 0)
    {
        return HandleInvalid;
    } 
    else 
    {
        auto image = ImageCreate(AllocatorGetHeap(), width, height,PixelFormat::Depth);
        image.Resolve()->setBuffer(pixelBuffer);
        return image;
    }
}

Vector2 TrueTypeFont::getKerning(int codepoint1, int codepoint2, int fontSize) const
{
    float scale = stbtt_ScaleForPixelHeight(&fontInfo->font, (float)fontSize);
    float x = scale*stbtt_GetCodepointKernAdvance(&fontInfo->font, codepoint1,codepoint2);
    return Vector2(x,0);
}

NAMESPACE_ENGINE_END
