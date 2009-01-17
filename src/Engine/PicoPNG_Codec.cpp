/************************************************************************
*
* vaporEngine by triton © (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/CompileOptions.h"

#ifdef VAPOR_IMAGE_CODEC_PICOPNG

#include "vapor/resources/PicoPNG_Codec.h"
#include "vapor/vfs/File.h"

using vapor::vfs::File;

/**
 * PicoPNG main decoding function declaration
 */
int decodePNG(std::vector<byte>& out_image_32bit, ulong& image_width, 
			  ulong& image_height, const byte* in_png, ulong in_size);

namespace vapor {
	namespace resources {

PicoPNG::PicoPNG()
{
	info("resources::image", "Registered picopng Image codec handler");

	_extensions.push_back("png");
}

Image& PicoPNG::decode(File& f)
{
	// TODO: check if file is open
	long size = f.getSize();

	// read contents of the file into the vector
	vector<byte> filebuf(size);
	f.read(&filebuf[0], size);

	ulong width, height;
	vector<byte> buffer;
	decodePNG(buffer, width, height, &filebuf[0], (ulong) filebuf.size());

	return *(new Image());
}

} } // end namespaces

#endif