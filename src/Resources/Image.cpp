/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Resources/API.h"
#include "Resources/Image.h"
#include "Core/Log.h"

#ifdef VAPOR_IMAGE_LODEPNG
	#include "lodepng.h"
#endif

namespace vapor {

//-----------------------------------//

BEGIN_ENUM(PixelFormat)
	ENUM(R8G8B8A8)
	ENUM(R8G8B8)
	ENUM(Depth)
	ENUM(DXT1)
	ENUM(DXT1a)
	ENUM(DXT3)
	ENUM(DXT5)
	ENUM(DXT5nm)
	ENUM(Unknown)
END_ENUM()

BEGIN_CLASS_PARENT(Image, Resource)
	FIELD_ENUM(PixelFormat, format) FIELD_READONLY(format)
END_CLASS()

//-----------------------------------//

Image::Image()
	: width(0)
	, height(0)
	, format(PixelFormat::Unknown)
{ }

//-----------------------------------//

Image::Image(int width, int height, PixelFormat::Enum format)
	: width(width)
	, height(height)
	, format(format)
{
	uint size = width*height;

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

#define PF(n) PixelFormat::n

bool Image::isCompressed() const
{
	switch(format)
	{
	case PF(DXT1):
	case PF(DXT1a):
	case PF(DXT3):
	case PF(DXT5):
	case PF(DXT5nm):
		return true;
	}

	return false;
}

//-----------------------------------//

void Image::setColor( const Color& color )
{
	if( format != PixelFormat::R8G8B8A8 )
		return;

	for( uint i = 0; i < buffer.size(); i += 4 )
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
	LogInfo( "Image has pixel format '%s' and size %dx%d", 
		 PixelFormat::getStaticType().getName(format).c_str(), width, height );
}

//-----------------------------------//

void ImageWriter::save( const ImagePtr& image, const std::string& filename )
{
#ifdef VAPOR_IMAGE_WRITER	
	// TODO: sleep until the image is not loadeds
	if( !image->isLoaded() )
		return;

	if( image->isCompressed() )
		output = image->getBuffer();
	else if( !convertPNG(image) )
		return;

	FileStream file(filename, StreamMode::Write);
	
	if( !file.open() )
		return;

	file.write(output);
#endif
}

//-----------------------------------//

bool ImageWriter::convertPNG( const ImagePtr& image )
{
#ifdef VAPOR_IMAGE_WRITER
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
		assert( 0 && "Pixel format is not supported" );
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

} // end namespace