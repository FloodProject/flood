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
	width = (size.x >= 0) ? size.x : 0;
	height = (size.y >= 0) ? size.y : 0;
	format = pixelFormat;
}

//-----------------------------------//

void Texture::setImage( const ImageHandle& imageHandle )
{
	this->image = imageHandle;

    Image* image = imageHandle.Resolve();

    uploaded = false;

    if (!image)
    {
        width = 0;
        height = 0;
        format = PixelFormat::Unknown;
        return;
    }

	width = image->getWidth();
	height = image->getHeight();
	format = image->getPixelFormat();
}

//-----------------------------------//

Image* Texture::readImage() const
{
	#pragma TODO("Add texture reading from the backend")
	return nullptr;
}

//-----------------------------------//

<<<<<<< HEAD
int32 Texture::getExpectedSize() const
{
	int32 size = width * height;
=======
int Texture::getExpectedSize() const
{
	int size = width * height;
>>>>>>> 15cb56c21ae8cc1ccee88b8e378337026546d87a

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