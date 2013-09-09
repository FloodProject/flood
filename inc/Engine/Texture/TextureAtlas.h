/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Engine/API.h"
#include "Graphics/Resources/Image.h"
#include "Engine/Texture/BinPacking.h"

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

    TextureAtlas(uint maxSize, PixelFormat pixelFormat);

    bool addImage(const ImageHandle& imageHandle);

    bool getImageSubTexture(const ImageHandle& imageHandle, SubTexture& subTexture);

    ImageHandle getAtlasImageHandle() const;

private:

    void resizeAtlas(uint newSize);

    void addImage(ImageHandle newImageHandle, Rectangle newRect);

    static const uint DefaultSize = 512;
    
    uint atlasSize;
    uint atlasMaxSize;
    MaxRectsBinPack rectanglePacker;

    struct CompareHandle {
		    bool operator()(const ImageHandle& h1,const ImageHandle& h2) const { return h1.id < h2.id; }
    };

    std::map<ImageHandle, SubTexture, CompareHandle> imageSubTextures;
    ImageHandle atlasImageHandle;
};

//-----------------------------------//

NAMESPACE_ENGINE_END