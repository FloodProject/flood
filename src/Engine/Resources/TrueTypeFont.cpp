/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Engine/API.h"
#include "Engine/Resources/TrueTypeFont.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

TrueTypeFont::TrueTypeFont(const String& font)
{
    unsigned char ttf_buffer[1<<25];
    unsigned char temp_bitmap[512*512];
    stbtt_bakedchar cdata[96]; // ASCII 32..126 is 95 glyphs
    fread(ttf_buffer, 1, 1<<25, fopen("c:/windows/fonts/times.ttf", "rb"));
    stbtt_BakeFontBitmap(ttf_buffer,0, 32.0, temp_bitmap,512,512, 32,96, cdata); // no guarantee this fits!


}

//-----------------------------------//

TrueTypeFont::~TrueTypeFont()
{
}

//-----------------------------------//

NAMESPACE_ENGINE_END