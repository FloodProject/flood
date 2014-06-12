/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Engine/API.h"

#ifdef ENABLE_IMAGE_PICOPNG

#include "Engine/Resources/PNG_Loader.h"

int decodePNG(Vector<byte>& out_image_32bit, uint32& image_width, 
              uint32& image_height, const byte* in_png, uint32 in_size);

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(PNG_Pico_Loader, ResourceLoader)
    FIELD_PRIMITIVE(bool, flipAxisY)
REFLECT_CLASS_END()

//-----------------------------------//

PNG_Pico_Loader::PNG_Pico_Loader()
    : flipAxisY(false)
{
    extensions.Push("png");
}

//-----------------------------------//

bool PNG_Pico_Loader::decode(ResourceLoadOptions& options)
{
    Vector<byte> data;
    StreamRead((Stream*) &stream, data);

    if( data.Empty() ) return false;

    uint32 width, height;
    Vector<byte> buffer;
    
    decodePNG(buffer, width, height, &data[0], data.Size());

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

void PNG_Pico_Loader::flip( Vector<byte>& buffer, uint32 width, uint32 height )
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

NAMESPACE_ENGINE_END

#endif