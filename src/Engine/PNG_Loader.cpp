/************************************************************************
*
* vaporEngine by triton © (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/CompileOptions.h"

#ifdef VAPOR_IMAGE_CODEC_PICOPNG

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

Image* PNG_Pico_Loader::decode(File& f)
{
	// TODO: check if file is open
	long size = f.getSize();

	// read contents of the file into the vector
	vector<byte> filebuf(size);
	f.read(&filebuf[0], size);

	ulong width, height;
	vector<byte> buffer;
	decodePNG(buffer, width, height, &filebuf[0], (ulong) filebuf.size());

	Image* image = new Image();
	return image;
}

//-----------------------------------//

} } // end namespaces

#endif