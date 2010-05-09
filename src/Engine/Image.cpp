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

namespace vapor { namespace resources {

//-----------------------------------//

namespace PixelFormat
{
	const std::string getString(Enum e) 
	{
		switch(e) 
		{
		case R8G8B8A8:
			return "R8G8B8A8";
		case R8G8B8:
			return "R8G8B8";
		default:
			return "(Unknown)";
		}
	}
}

//-----------------------------------//

Image::Image()
	: pixelFormat( PixelFormat::Unknown ),
	width(0), height(0)
{

}

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
		png.colorType = 2;
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
	
	encoder.encode(output, buffer, width, height);

	if( encoder.hasError() )
	{
		warn("image", "Could not write PNG: '%s'", filename.c_str() );
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
		PixelFormat::getString(getPixelFormat()).c_str(), 
		getWidth(), getHeight());
}

//-----------------------------------//

} } // end namespaces