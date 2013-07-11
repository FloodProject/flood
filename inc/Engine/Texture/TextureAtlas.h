/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Engine/API.h"
#include "Graphics/Resources/Image.h"
#include "Engine/RectangleBinPack/MaxRectsBinPack.h"

NAMESPACE_ENGINE_BEGIN

class TextureAtlas;

struct SubTexture
{
    Vector2 leftTopUV;
    Vector2 rightTopUV;
    Vector2 rightBottomUV;
    Vector2 leftBottomUV;
    TextureAtlas* atlas;
};


//-----------------------------------//

//TODO update atlas on Image modifications (delete, resize, buffer change)

class API_ENGINE TextureAtlas
{
public:

    TextureAtlas(uint maxSize);

    bool addImage(const ImageHandle& imageHandle);

    bool getImageSubTexture(const ImageHandle& imageHandle, SubTexture& subTexture);

    ImageHandle getAtlasImageHandle() const;

private:

    void resizeAtlas(uint newSize);

    void addImage(ImageHandle newImageHandle, Rect newRect);

    static const uint DefaultSize = 512;
    
    uint atlasSize;
    uint atlasMaxSize;
    MaxRectsBinPack rectanglePacker;

    Hash<SubTexture> imageSubTextures; // keyed by Handle::id
    ImageHandle atlasImageHandle;
};

//-----------------------------------//

NAMESPACE_ENGINE_END