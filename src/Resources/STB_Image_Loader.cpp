/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Resources/API.h"

#ifdef ENABLE_IMAGE_STB

#include "Resources/STB_Image_Loader.h"
#include "Core/Log.h"

#define STBI_HEADER_FILE_ONLY
#include "stb_image.cpp"

NAMESPACE_RESOURCES_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(STB_Image_Loader, ResourceLoader)
REFLECT_CLASS_END()

//-----------------------------------//

STB_Image_Loader::STB_Image_Loader()
{
	extensions.push_back("png");
	extensions.push_back("jpg");
	extensions.push_back("bmp");
	extensions.push_back("tga");
}

//-----------------------------------//

bool STB_Image_Loader::decode(ResourceLoadOptions& options)
{
	std::vector<uint8> data;
	StreamRead(options.stream, data);

	if( data.empty() ) return false;

	int width, height, comp;
	
	byte* pixelData = stbi_load_from_memory(
		&data[0], data.size(), &width, &height,
		&comp, 0 /* 0=auto-detect, 3=RGB, 4=RGBA */ );

	if( !pixelData )
	{
		const char* error = stbi_failure_reason();
		LogError("STB image error: %s", error);
		return false;
	}

	// Build our image with the pixel data returned by stb_image.
	PixelFormat::Enum pf = PixelFormat::Unknown;
	
	switch( comp )
	{
	case 0:
	case 1: break;
	case 3: pf = PixelFormat::R8G8B8; break;
	case 4: pf = PixelFormat::R8G8B8A8; break;
	}

	if( pf == PixelFormat::Unknown )
	{
		LogError( "Implement support for more pixel formats" );
		return false;
	}
	
	std::vector<byte> buffer;
	uint32 size = width*height*comp; 
	buffer.resize(size);
	
	memcpy(&buffer[0], pixelData, size);
	free(pixelData);

	Image* image = static_cast<Image*>( options.resource );
	image->setWidth( width );
	image->setHeight( height );
	image->setPixelFormat( pf );
	image->setBuffer( buffer );

	return true;
}

//-----------------------------------//

NAMESPACE_RESOURCES_END

#endif