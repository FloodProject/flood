/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

#ifdef VAPOR_IMAGE_STB

#include "vapor/resources/STB_Image_Loader.h"

#define STBI_HEADER_FILE_ONLY
#include "stb_image.cpp"

using vapor::vfs::File;

//-----------------------------------//

namespace vapor { namespace resources {

//-----------------------------------//

STB_Image_Loader::STB_Image_Loader()
{
	extensions.push_back("png");
	extensions.push_back("jpg");
	extensions.push_back("bmp");
	extensions.push_back("tga");
}

//-----------------------------------//

Image* STB_Image_Loader::decode(const File& file)
{
	PROFILE;

	// read contents of the file into the vector
	std::vector<byte> filebuf = file.read();

	// TODO: error handling
	int width, height, comp;
	byte* data = stbi_load_from_memory( &filebuf[0], filebuf.size(), 
		&width, &height, &comp, 0 /* 0=auto-detect, 3=RGB, 4=RGBA */ );

	// build our image with the data. the pixel format returned by stb_image

	PixelFormat::Enum pf = PixelFormat::Unknown;
	switch( comp )
	{
	case 0:
	case 1:
		error( "STB_Image", "Implement support for more pixel formats" );
		return nullptr;
	case 3:
		pf = PixelFormat::R8G8B8;
		break;
	case 4:
		pf = PixelFormat::R8G8B8A8;
		break;
	}

	Image* image = new Image( static_cast<ushort>(width), 
		static_cast<ushort>(height), pf );
	
	std::vector<byte> buffer;
	int sz = width*height*comp; 
	buffer.resize(sz);
	memcpy(&buffer[0], data, sz);

	free(data);

	image->setBuffer( buffer );
	image->setURI( file.getPath() );

	return image;
}

//-----------------------------------//

} } // end namespaces

#endif