/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/CompileOptions.h"

#ifdef VAPOR_IMAGE_PICOPNG

#include "vapor/resources/PNG_Loader.h"

using vapor::vfs::File;

//-----------------------------------//

/**
 * PicoPNG decoding function declaration
 */

int decodePNG(std::vector<byte>& out_image_32bit, ulong& image_width, 
			  ulong& image_height, const byte* in_png, ulong in_size);

//-----------------------------------//

namespace vapor {
	namespace resources {

//-----------------------------------//

PNG_Pico_Loader::PNG_Pico_Loader()
{
	extensions.push_back("png");
}

//-----------------------------------//

Image* PNG_Pico_Loader::decode(File& file)
{
	// TODO: check if file is open
	long size = file.getSize();

	// read contents of the file into the vector
	std::vector<byte> filebuf(size);
	file.read(&filebuf[0], size);

	ulong width, height;
	std::vector<byte>& buffer = *new std::vector<byte>();
	decodePNG(buffer, width, height, &filebuf[0], (ulong) filebuf.size());

	// build our image with the data. the pixel format return by picoPNG
	// is always the same, 32bits per pixel, RGBA 8 bits per component.
	Image* image = new Image(width, height, PixelFormat::R8G8B8A8);
	image->setBuffer(buffer);

	return image;
}

//-----------------------------------//

} } // end namespaces

#endif