/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"

#ifdef VAPOR_IMAGE_PICOPNG

#include "vapor/resources/PNG_Loader.h"

using vapor::File;

//-----------------------------------//

// PicoPNG decoding function forward declaration.
int decodePNG(std::vector<byte>& out_image_32bit, ulong& image_width, 
			  ulong& image_height, const byte* in_png, ulong in_size);

//-----------------------------------//

namespace vapor {

//-----------------------------------//

PNG_Pico_Loader::PNG_Pico_Loader()
{
	extensions.push_back("png");
}

//-----------------------------------//

bool PNG_Pico_Loader::decode(const File& file, Resource* res)
{
	// read contents of the file into the vector
	std::vector<byte> filebuf = file.read();

	ulong width, height;
	std::vector<byte> buffer;
	
	decodePNG(buffer, width, height, &filebuf[0], filebuf.size());

	//flip( buffer, width, height );

	// build our image with the data. the pixel format returned by picoPNG
	// is always the same, 32bits per pixel, RGBA 8 bits per component.
	Image* image = static_cast<Image*>( res );
	image->setWidth( width );
	image->setHeight( height );
	image->setPixelFormat( PixelFormat::R8G8B8A8 );
	image->setBuffer( buffer );

	return true;
}

//-----------------------------------//

void PNG_Pico_Loader::flip( std::vector<byte>& buffer, ulong width, ulong height )
{
	// flip Y in place
	for( uint y = 0; y < height/2; y++ )
	{
		int minrow = y*width*4;
		int maxrow = width*(height-y-1)*4;
 
		for( uint x = 0; x < width*4; x++ )
		{
			std::swap( buffer[minrow+x], buffer[maxrow+x] );
		}
	}
}

//-----------------------------------//

} // end namespace

#endif