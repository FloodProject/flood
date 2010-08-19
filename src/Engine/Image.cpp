/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/resources/Image.h"
#include "lodepng.h"

namespace vapor {

//-----------------------------------//

BEGIN_ENUM(PixelFormat)
	ENUM(R8G8B8A8)
	ENUM(R8G8B8)
	ENUM(Depth)
	ENUM(Unknown)
END_ENUM()

BEGIN_CLASS_PARENT(Image, Resource)
END_CLASS()

//-----------------------------------//

Image::Image()
	: width(0)
	, height(0)
	, pixelFormat( PixelFormat::Unknown )
{ }

//-----------------------------------//

Image::Image(ushort width, ushort height, PixelFormat::Enum format)
	: width(width)
	, height(height)
	, pixelFormat(format)
{ }

//-----------------------------------//

void Image::save( const std::string& filename )
{
	// TODO: sleep until the image is not loaded
	while( !isLoaded() )
		;

	std::vector<byte> output;
	
	LodePNG::Encoder encoder;
	
	LodePNG_InfoColor& png = encoder.getInfoPng().color;
	LodePNG_InfoColor& raw = encoder.getInfoRaw().color;
	
	switch( pixelFormat )
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
	encoder.encode(output, buffer, width, height);

	if( encoder.hasError() )
	{
		error("image", "Could not write PNG: '%s'", filename.c_str() );
		return;
	}

	NativeFile file(filename, AccessMode::Write);
	
	if( !file.open() )
		return;

	file.write(output);
}

//-----------------------------------//

void Image::log() const
{
	info("Image", "Image has pixel format '%s' and size %dx%d", 
		 PixelFormat::getType().getString(pixelFormat), width, height);
}

//-----------------------------------//

} // end namespace