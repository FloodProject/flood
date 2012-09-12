/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Graphics/API.h"
#include "Graphics/Resources/Image.h"
#include "Core/Log.h"
#include "Core/Stream.h"

//#define ENABLE_IMAGE_WRITER
//#define ENABLE_IMAGE_LODEPNG

#ifdef ENABLE_IMAGE_LODEPNG
	#include "lodepng.h"
#endif

NAMESPACE_GRAPHICS_BEGIN

//-----------------------------------//

REFLECT_ENUM(PixelFormat)
	ENUM(R8G8B8A8)
	ENUM(R8G8B8)
	ENUM(Depth)
	ENUM(DXT1)
	ENUM(DXT1a)
	ENUM(DXT3)
	ENUM(DXT5)
	ENUM(DXT5nm)
	ENUM(Unknown)
REFLECT_ENUM_END()
 
REFLECT_CHILD_CLASS(Image, Resource)
	FIELD_ENUM(4, PixelFormat, format) FIELD_READONLY(format)
REFLECT_CLASS_END()

//-----------------------------------//

ImageHandle ImageCreate(Allocator* alloc, uint32 width, uint32 height, PixelFormat format)
{
	Image* image = Allocate(alloc, Image);
	image->create(width, height, format);

	return HandleCast<Image>( ResourceHandleCreate(image) );
}

//-----------------------------------//

Image::Image()
	: width(0)
	, height(0)
	, format(PixelFormat::Unknown)
{ }

//-----------------------------------//

Image::Image(uint32 _width, uint32 _height, PixelFormat _format)
	: width(0)
	, height(0)
	, format(PixelFormat::Unknown)
{
	create(_width, _height, _format);
}

//-----------------------------------//

void Image::create(uint32 _width, uint32 _height, PixelFormat _format)
{
	this->width  = _width;
	this->height = _height;
	this->format = _format;

	uint32 size = width*height;

	switch(format)
	{
	case PixelFormat::R8G8B8A8:
		size = size*4;
		break;
	case PixelFormat::R8G8B8:
		size = size*3;
		break;
	case PixelFormat::Depth:
		size = size;
		break;
	}

	buffer.resize(size);
}

//-----------------------------------//

bool Image::isCompressed() const
{
	switch(format)
	{
	case PixelFormat::DXT1:
	case PixelFormat::DXT1a:
	case PixelFormat::DXT3:
	case PixelFormat::DXT5:
	case PixelFormat::DXT5nm:
		return true;
	}

	return false;
}

//-----------------------------------//

void Image::setColor( const Color& color )
{
	if( format != PixelFormat::R8G8B8A8 )
		return;

	for( size_t i = 0; i < buffer.size(); i += 4 )
	{
		buffer[i+0] = byte(color.r * 255);
		buffer[i+1] = byte(color.g * 255);
		buffer[i+2] = byte(color.b * 255);
		buffer[i+3] = 1;
	}
}

//-----------------------------------//

void Image::log() const
{
	const char* desc = EnumGetValueName(PixelFormatGetType(), (int32)format);
	LogInfo( "Image has pixel format '%s' and size %dx%d", desc, width, height );
}

//-----------------------------------//

void ImageWriter::save( Image* image, Stream* stream )
{
#ifdef ENABLE_IMAGE_WRITER
	if( !stream ) return;

	// TODO: sleep until the image is not loaded
	if( !image->isLoaded() ) return;

	if( image->isCompressed() )
		output = image->getBuffer();
	else if( !convert(image) )
		return;

	if( output.empty() ) return;

	StreamWrite(stream, output.data(), output.size());
#endif
}

//-----------------------------------//

bool ImageWriter::convert( Image* image )
{
    #pragma TODO("Fix the LodePNG image writing code to the new API")

#if defined(ENABLE_IMAGE_WRITER_)
	LodePNG::Encoder encoder;
	
	LodePNG_InfoColor& png = encoder.getInfoPng().color;
	LodePNG_InfoColor& raw = encoder.getInfoRaw().color;
	
	switch(image->getPixelFormat())
	{
	case PixelFormat::Depth:
		png.colorType = 0;
		raw.colorType = 0;
		break;
	case PixelFormat::R8G8B8:
		png.colorType = 2;
		raw.colorType = 2;
		break;
	case PixelFormat::R8G8B8A8:
		png.colorType = 6;
		raw.colorType = 6;
		break;
	default:
		LogError("Pixel format is not supported");
		return false;
	}

	raw.bitDepth = 8;
	
	encoder.encode(output, image->getBuffer(),
		image->getWidth(), image->getHeight());

	if( encoder.hasError() )
	{
		LogError("Could not encode image to PNG");
		return false;
	}

	return true;
#else
	return false;
#endif
}

//-----------------------------------//

NAMESPACE_GRAPHICS_END