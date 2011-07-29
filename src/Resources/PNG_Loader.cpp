/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Resources/API.h"

#ifdef ENABLE_IMAGE_PICOPNG

#include "Resources/PNG_Loader.h"

int decodePNG(std::vector<byte>& out_image_32bit, uint32& image_width, 
			  uint32& image_height, const byte* in_png, uint32 in_size);

NAMESPACE_RESOURCES_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(PNG_Pico_Loader, ResourceLoader)
	FIELD_PRIMITIVE(bool, flipAxisY)
REFLECT_CLASS_END()

//-----------------------------------//

PNG_Pico_Loader::PNG_Pico_Loader()
	: flipAxisY(false)
{
	extensions.push_back("png");
}

//-----------------------------------//

bool PNG_Pico_Loader::decode(const Stream& stream, Resource* res)
{
	std::vector<byte> data;
	StreamRead((Stream*) &stream, data);

	if( data.empty() ) return false;

	uint32 width, height;
	std::vector<byte> buffer;
	
	decodePNG(buffer, width, height, &data[0], data.size());

	//flip( buffer, width, height );

	// Build our image with the data. The pixel format returned by picoPNG
	// is always the same, 32bits per pixel, RGBA 8 bits per component.

	Image* image = static_cast<Image*>( res );
	image->setPixelFormat( PixelFormat::R8G8B8A8 );
	image->setWidth( width );
	image->setHeight( height );
	image->setBuffer( buffer );

	return true;
}

//-----------------------------------//

void PNG_Pico_Loader::flip( std::vector<byte>& buffer, uint32 width, uint32 height )
{
	// Flip Y in place.
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

NAMESPACE_RESOURCES_END

#endif