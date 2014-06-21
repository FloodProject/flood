/************************************************************************
*
* Flood Project � (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Engine/API.h"
#include "Engine/Texture/TextureAtlas.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

static const MaxRectsBinPack::FreeRectChoiceHeuristic gs_heuristic = 
             MaxRectsBinPack::FreeRectChoiceHeuristic::RectBestAreaFit;

TextureAtlas::TextureAtlas(uint maxSize, PixelFormat pixelFormat)
{
    atlasMaxSize = maxSize;
    atlasSize = std::min(maxSize, DefaultSize);
    rectanglePacker.Init(atlasSize,atlasSize);
    atlasImageHandle = ImageCreate(AllocatorGetHeap(),atlasSize,atlasSize,pixelFormat);
}

static const int RBLOCK = 96;

// We use this function to rotate images in-place since sometimes
// the packing algorithm will rotate the image to have a better fit.
static void RotateImage(Image* srcImage, Image* dstImage, Vector2i dstOffset)
{
    assert(srcImage->getPixelFormat() == dstImage->getPixelFormat());

    int bpp = srcImage->getPixelSize();

    int srcWidth = srcImage->getWidth();
    int srcHeight = srcImage->getHeight();
    int dstWidth = srcHeight;
    int dstHeight = srcWidth;

    // get src and dst scan width
    int srcPitch  = srcWidth*bpp;
    int dstPitch  = dstImage->getWidth()*bpp;

    byte* bsrc = srcImage->getBuffer().Buffer();
    byte* bdst = dstImage->getBuffer().Buffer();

    for(int xs = 0; xs < dstWidth; xs += RBLOCK) {    // for all image blocks of RBLOCK*RBLOCK pixels
        for(int ys = 0; ys < dstHeight; ys += RBLOCK) {
            for(int y = ys; y < std::min(dstHeight, ys + RBLOCK); y++) {    // do rotation
                int y2 = dstHeight - y - 1;
                // point to src pixel at (y2, xs)
                byte* src_bits = bsrc + (xs * srcPitch) + (y2 * bpp);
                // point to dst pixel at (xs, y)
                byte* dst_bits = bdst + (( y + dstOffset.y) * dstPitch) + ((xs + dstOffset.x) * bpp);
                for (int x = xs; x < std::min(dstWidth, xs + RBLOCK); x++) {
                    // dst.SetPixel(x, y, src.GetPixel(y2, x));
                    for(int j = 0; j < bpp; j++) {
                        dst_bits[j] = src_bits[j];
                    }
                    dst_bits += bpp;
                    src_bits += srcPitch;
                }
            }
        }
    }

    dstImage->SetModified();
}

bool TextureAtlas::addImage(const ImageHandle& newImageHandle) 
{
    if(imageSubTextures.Find( newImageHandle ) != imageSubTextures.End())
        return true;

    Image* newImage = newImageHandle.Resolve();

    Rectangle newRect = rectanglePacker.Insert(newImage->getWidth()+1, newImage->getHeight()+1, gs_heuristic);

    if (newRect.height == 0 || newRect.width == 0){
        if (atlasSize >= atlasMaxSize)
            return false;

        atlasSize = std::min(atlasSize*2, atlasMaxSize);
        resizeAtlas(atlasSize);

        return addImage(newImageHandle);
    }

    newRect.width--;
    newRect.height--;

    addImage(newImageHandle,newRect);

    return true;
}

bool TextureAtlas::getImageSubTexture(const ImageHandle& imageHandle, SubTexture& subTexture)
{
    if(imageSubTextures.Find( imageHandle ) == imageSubTextures.End())
        return false;

    subTexture = imageSubTextures[imageHandle];
    return true;
}

ImageHandle TextureAtlas::getAtlasImageHandle() const
{
    return atlasImageHandle;
}


void TextureAtlas::resizeAtlas(uint newSize)
{
    rectanglePacker.Init(newSize,newSize);

    Vector<Vector2i> rectSizes;
    Vector<Rectangle> newRects;

    Image* atlasImage = atlasImageHandle.Resolve();

    HashMap<ImageHandle, SubTexture>::Iterator iter;
    for (iter = imageSubTextures.Begin(); iter != imageSubTextures.End(); ++iter) {
        Vector2i rectSize;
        int width = (iter->second.rightBottomUV.x - iter->second.leftTopUV.x)*width;
        int height = (iter->second.rightBottomUV.y - iter->second.leftTopUV.y)*height;
        
        rectSize.x = width+1;
        rectSize.y = height+1;

        rectSizes.Push(rectSize);
    }

    rectanglePacker.Insert(rectSizes, newRects, gs_heuristic);

    assert(newRects.Size() == imageSubTextures.Size());

    int i;
    for (i = 0, iter = imageSubTextures.Begin(); iter != imageSubTextures.End(); ++iter, ++i) 
    {
        ImageHandle newImageHandle = iter->first;
        Rectangle newRect = newRects[i];

        newRect.width--;
        newRect.height--;

        addImage(newImageHandle, newRect);
    }
}

void TextureAtlas::addImage(ImageHandle newImageHandle, Rectangle newRect)
{
    Image* newImage = newImageHandle.Resolve();

    bool wasRotated = newImage->getWidth() == newRect.height &&
                      newImage->getHeight() == newRect.width;

    Image* atlasImage = atlasImageHandle.Resolve();

    assert(newImage->getPixelFormat() == atlasImage->getPixelFormat());

    if (wasRotated)
        RotateImage(newImage,atlasImage,Vector2i(newRect.x,newRect.y));
    else
        atlasImage->setBuffer(newImage,Vector2i(newRect.x,newRect.y));

    SubTexture subTexture;
    subTexture.atlas = this;

    float bottom = (float)(newRect.y+newRect.height)/atlasSize;
    float top = (float)newRect.y/atlasSize;
    float right = (float)(newRect.x+newRect.width)/atlasSize;
    float left = (float)newRect.x/atlasSize;

    if(!wasRotated)
    {
        subTexture.leftTopUV = Vector2(left,top);
        subTexture.rightTopUV = Vector2(right,top);
        subTexture.rightBottomUV = Vector2(right,bottom);
        subTexture.leftBottomUV = Vector2(left,bottom);
    } 
    else 
    {
        subTexture.rightTopUV = Vector2(left,top);
        subTexture.rightBottomUV = Vector2(right,top);
        subTexture.leftBottomUV = Vector2(right,bottom);
        subTexture.leftTopUV = Vector2(left,bottom);
    }

    imageSubTextures[newImageHandle] = subTexture;
}

//-----------------------------------//

NAMESPACE_ENGINE_END