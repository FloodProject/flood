/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Engine/API.h"
#include "Engine/Resources/TrueTypeFont.h"

#include "Engine/RectangleBinPack/Rect.h"
#include "Engine/Texture/TextureAtlas.h"
#include "stb_truetype.h"

NAMESPACE_ENGINE_BEGIN

struct TrueTypeFont::FontInfo {
    stbtt_fontinfo font;
};

//-----------------------------------//

TrueTypeFont::TrueTypeFont(const String& fontName)
{
    fontInfo = AllocateThis(FontInfo);
    fread(ttf_buffer, 1, 1<<25, fopen("c:/windows/fonts/arial.ttf", "rb"));
    stbtt_InitFont(&fontInfo->font, ttf_buffer, 0);

    lineHeigth = 15;
    scale = stbtt_ScaleForPixelHeight(&fontInfo->font, lineHeigth);

    int ascent;
    stbtt_GetFontVMetrics(&fontInfo->font, &ascent,0,0);
    baseLine = ascent * scale;

}

//-----------------------------------//

TrueTypeFont::~TrueTypeFont()
{
    Deallocate(fontInfo);
}

//-----------------------------------//

bool TrueTypeFont::createGlyph(int codepoint, Glyph& glyph) const
{
    int width,height;
    byte* pixelBuffer;

    pixelBuffer = stbtt_GetCodepointBitmap(&fontInfo->font, 0, scale, codepoint, &width, &height, 0,0);

    if (width == 0 || height == 0)
    {
        glyph.image = HandleInvalid;
    } 
    else 
    {
        glyph.image = ImageCreate(AllocatorGetHeap(), width, height,PixelFormat::Depth);
        glyph.image.Resolve()->setBuffer(pixelBuffer, width*height);
    }

    int x0,x1,y0,y1;
    stbtt_GetCodepointBitmapBoxSubpixel(&fontInfo->font, codepoint, scale,scale,0,0, &x0,&y0,&x1,&y1);
    glyph.baseLineOffset = baseLine + y0;

    int advance;
    stbtt_GetCodepointHMetrics(&fontInfo->font, codepoint, &advance, 0);
    glyph.advance = advance*scale;

    return true;
}

Vector2i TrueTypeFont::getKerning(int codepoint1, int codepoint2) const
{
    float x = scale*stbtt_GetCodepointKernAdvance(&fontInfo->font, codepoint1,codepoint2);
    return Vector2i(x,0);
}

NAMESPACE_ENGINE_END