/************************************************************************
*
* Flood Project © (2008-201x)
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

TextureAtlas::TextureAtlas(uint maxSize)
{
    atlasMaxSize = maxSize;
    atlasSize = std::min(maxSize, DefaultSize);
    rectanglePacker.Init(atlasSize,atlasSize);
    atlasImageHandle = ImageCreate(AllocatorGetHeap(),atlasSize,atlasSize,PixelFormat::R8G8B8A8);
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

    byte* bsrc = srcImage->getBuffer().data();
    byte* bdst = dstImage->getBuffer().data();

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
}

bool TextureAtlas::addImage(const ImageHandle& newImageHandle) 
{
    if(imageSubTextures.find( newImageHandle ) != imageSubTextures.end())
        return true;

    Image* newImage = newImageHandle.Resolve();

    Rect newRect = rectanglePacker.Insert(newImage->getWidth(), newImage->getHeight(), gs_heuristic);

    if (newRect.height == 0 || newRect.width == 0){
        if (atlasSize >= atlasMaxSize)
            return false;

        atlasSize = std::min(atlasSize*2, atlasMaxSize);
        resizeAtlas(atlasSize);

        return addImage(newImageHandle);
    }

    addImage(newImageHandle,newRect);

    return true;
}

bool TextureAtlas::getImageSubTexture(const ImageHandle& imageHandle, SubTexture& subTexture)
{
    if(imageSubTextures.find( imageHandle ) == imageSubTextures.end())
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

    std::vector<Vector2i> rectSizes;
    std::vector<Rect> newRects;

    Image* atlasImage = atlasImageHandle.Resolve();

    std::map<ImageHandle, SubTexture>::iterator iter;
    for (iter = imageSubTextures.begin(); iter != imageSubTextures.end(); ++iter) {
        Vector2i rectSize;
        int width = (iter->second.rightBottomUV.x - iter->second.leftTopUV.x)*width;
        int height = (iter->second.rightBottomUV.y - iter->second.leftTopUV.y)*height;
        rectSize.x = width;
        rectSize.y = height;
        rectSizes.push_back(rectSize);
    }

    rectanglePacker.Insert(rectSizes, newRects, gs_heuristic);

    assert(newRects.size() == imageSubTextures.size());

    int i;
    for (i = 0, iter = imageSubTextures.begin(); iter != imageSubTextures.end(); ++iter, ++i) 
    {
        ImageHandle newImageHandle = iter->first;
        Rect newRect = newRects[i];

        addImage(newImageHandle, newRect);
    }
}

void TextureAtlas::addImage(ImageHandle newImageHandle, Rect newRect)
{
    Image* newImage = newImageHandle.Resolve();

    bool wasRotated = newImage->getWidth() == newRect.height &&
                      newImage->getHeight() == newRect.width;

    Image* atlasImage = atlasImageHandle.Resolve();
    if (newImage->getPixelFormat() == atlasImage->getPixelFormat())
    {
        atlasImage->setBuffer(newImage,Vector2i(newRect.x,newRect.y));
    } 
    else 
    {
         assert(newImage->getPixelFormat() == PixelFormat::Depth &&
                atlasImage->getPixelFormat() == PixelFormat::R8G8B8A8);

         Image tmpImage(newImage->getWidth(),newImage->getHeight(),PixelFormat::R8G8B8A8);

         int newImageSize = newImage->getSize();
         std::vector<byte>& buffer = tmpImage.getBuffer();
         buffer.resize(newImageSize*4,0);

         for (int i = 0; i < newImageSize; i++)
         {
             buffer[i*4+3] = newImage->getBuffer()[i]; //A
         }

         if (wasRotated)
            RotateImage(&tmpImage,atlasImage,Vector2i(newRect.x,newRect.y));
         else
            atlasImage->setBuffer(&tmpImage,Vector2i(newRect.x,newRect.y));
    }

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