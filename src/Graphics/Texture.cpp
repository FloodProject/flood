/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Graphics/API.h"
#include "Graphics/Texture.h"

NAMESPACE_GRAPHICS_BEGIN

//-----------------------------------//

Texture::Texture()
    : id(0)
    , uploaded(false)
    , uploadedImageTimestamp(0)
    , image(nullptr)
    , anisotropicFilter(1.0f)
    , target(TextureTarget::Target2D)
{
}

//-----------------------------------//

Texture::~Texture()
{

}

//-----------------------------------//

void Texture::allocate(const Vector2i& size, PixelFormat pixelFormat)
{
	width = (uint16) size.x;
	height = (uint16) size.y;
	format = pixelFormat;
}

//-----------------------------------//

void Texture::setImage( Image* image )
{
	assert( image != nullptr );

	this->image = image;

	width = (uint16) image->getWidth();
	height = (uint16) image->getHeight();
	format = image->getPixelFormat();

    uploaded = false;
}

//-----------------------------------//

Image* Texture::readImage() const
{
	#pragma TODO("Add texture reading from the backend")
	return nullptr;
}

//-----------------------------------//

uint Texture::getExpectedSize() const
{
	uint size = width * height;

	switch( format )
	{
	case PixelFormat::R8G8B8A8: return size*4;
	case PixelFormat::R8G8B8: return size*3;
	case PixelFormat::Depth: return size;
	}

	assert( 0 && "This should not be reached" );
	return 0;
}

//-----------------------------------//

bool Texture::isUploaded() const
{
    if (image)
        return uploadedImageTimestamp == image->getTimestamp();

    return uploaded;
}

//-----------------------------------//

void Texture::setUploaded()
{
    if (image)
        uploadedImageTimestamp = image->getTimestamp();

    uploaded = true;
}

//-----------------------------------//

NAMESPACE_GRAPHICS_END