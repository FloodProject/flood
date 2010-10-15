/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"

#ifdef VAPOR_IMAGE_STB

#include "vapor/resources/STB_Image_Loader.h"

#define STBI_HEADER_FILE_ONLY
#include "stb_image.cpp"

using vapor::File;

namespace vapor {

//-----------------------------------//

STB_Image_Loader::STB_Image_Loader()
{
	extensions.push_back("png");
	extensions.push_back("jpg");
	extensions.push_back("bmp");
	extensions.push_back("tga");
}

//-----------------------------------//

bool STB_Image_Loader::decode(const File& file, Resource* res)
{
	// read contents of the file into the vector
	std::vector<byte> filebuf = file.read();

	if( filebuf.size() == 0 ) 
		return false;

	// TODO: error handling
	int width, height, comp;
	byte* data = stbi_load_from_memory( &filebuf[0], filebuf.size(), 
		&width, &height, &comp, 0 /* 0=auto-detect, 3=RGB, 4=RGBA */ );

	// Build our image with the pixel data returned by stb_image.

	PixelFormat::Enum pf = PixelFormat::Unknown;
	switch( comp )
	{
	case 0:
	case 1:
		Log::error( "Implement support for more pixel formats" );
		return false;
	case 3:
		pf = PixelFormat::R8G8B8;
		break;
	case 4:
		pf = PixelFormat::R8G8B8A8;
		break;
	}

	if( pf == PixelFormat::Unknown )
		return nullptr;
	
	std::vector<byte> buffer;
	int sz = width*height*comp; 
	buffer.resize(sz);
	memcpy(&buffer[0], data, sz);
	free(data);

	Image* image = static_cast<Image*>( res );
	image->setWidth( width );
	image->setHeight( height );
	image->setPixelFormat( pf );
	image->setBuffer( buffer );

	return image;
}

//-----------------------------------//

} // end namespace

#endif