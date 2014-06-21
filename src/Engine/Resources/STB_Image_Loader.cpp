/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Engine/API.h"

#ifdef ENABLE_IMAGE_STB

#include "Engine/Resources/STB_Image_Loader.h"
#include "Core/Log.h"

#define STBI_HEADER_FILE_ONLY
#include "stb_image.c"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(STB_Image_Loader, ResourceLoader)
REFLECT_CLASS_END()

//-----------------------------------//

STB_Image_Loader::STB_Image_Loader()
{
    extensions.Push("png");
    extensions.Push("jpg");
    extensions.Push("bmp");
    extensions.Push("tga");
}

//-----------------------------------//

bool STB_Image_Loader::decode(ResourceLoadOptions& options)
{
    Vector<uint8> data;
    options.stream->read(data);

    if( data.Empty() ) return false;

    int width, height, comp;
    
    byte* pixelData = stbi_load_from_memory(
        &data[0], data.Size(), &width, &height,
        &comp, 0 /* 0=auto-detect, 3=RGB, 4=RGBA */ );

    if( !pixelData )
    {
        const char* error = stbi_failure_reason();
        LogError("STB image error: %s", error);
        return false;
    }

    // Build our image with the pixel data returned by stb_image.
    PixelFormat pf = PixelFormat::Unknown;
    
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
    
    Vector<byte> buffer;
    uint32 size = width*height*comp; 
    buffer.Resize(size);
    
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

NAMESPACE_ENGINE_END

#endif