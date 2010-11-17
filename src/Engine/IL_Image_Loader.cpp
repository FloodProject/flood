/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"

#ifdef VAPOR_IMAGE_DEVIL

#include "vapor/resources/IL_Image_Loader.h"

#define IL_STATIC_LIB
#include "IL/il.h"
#include "IL/config.h"

namespace vapor {

//-----------------------------------//

IL_Image_Loader::IL_Image_Loader()
{
	ilInit();
	Log::info("Initialized DevIL");

#ifndef IL_NO_PNG
	extensions.push_back("png");
#endif

#ifndef IL_NO_JPG
	extensions.push_back("jpg");
#endif

#ifndef IL_NO_BMP
	extensions.push_back("bmp");
#endif
}

//-----------------------------------//

IL_Image_Loader::~IL_Image_Loader()
{
	ilShutDown();
}

//-----------------------------------//

bool IL_Image_Loader::decode(const Stream& stream, Resource* res)
{
	std::vector<byte> data;
	stream.read(data);

	if( data.empty() ) 
		return false;

	THREAD(boost::lock_guard<boost::mutex> lock(mutex);)

	ILuint id = ilGenImage();
	ilBindImage(id);

	ilEnable(IL_ORIGIN_SET);
	ilSetInteger(IL_ORIGIN_MODE, IL_ORIGIN_UPPER_LEFT);

	if( !ilLoadL(IL_TYPE_UNKNOWN, &data[0], data.size()) )
		return false;

	if( checkErrors() )
		return false;

	ILint type = ilGetInteger( IL_IMAGE_TYPE );
	ILint format = ilGetInteger( IL_IMAGE_FORMAT );
	ILint width = ilGetInteger( IL_IMAGE_WIDTH );
	ILint height = ilGetInteger( IL_IMAGE_HEIGHT );
	ILint depth = ilGetInteger( IL_IMAGE_DEPTH );
	ILint bpp = ilGetInteger( IL_IMAGE_BYTES_PER_PIXEL );

	if(type != IL_UNSIGNED_BYTE)
	{
		ilConvertImage(format, IL_UNSIGNED_BYTE);
		type = IL_UNSIGNED_BYTE;
	}

	// Build our image with the pixel data.
	PixelFormat::Enum pf = PixelFormat::Unknown;
	switch( format )
	{
	case IL_BGR:
		pf = PixelFormat::B8G8R8;
		break;
	case IL_BGRA:
		pf = PixelFormat::B8G8R8A8;
		break;
	case IL_RGB:
		pf = PixelFormat::R8G8B8;
		break;
	case IL_RGBA:
		pf = PixelFormat::R8G8B8A8;
		break;
	default:
		Log::error( "Implement support for more pixel formats" );
		return false;
	}

	Image* image = static_cast<Image*>( res );
	image->setWidth( width );
	image->setHeight( height );
	image->setPixelFormat( pf );

	//const std::vector<byte>& cbuffer = image->getBuffer();
	//std::vector<byte>& buffer = const_cast<std::vector<byte>&>(cbuffer);
	std::vector<byte> buffer;
	
	int size = width*height*bpp;
	buffer.resize(size);

	if( buffer.empty() )
		return false;

	ilCopyPixels(0, 0, 0, width, height, depth, format, type, &buffer[0]);

	if( checkErrors() )
		return false;

	image->setBuffer(buffer);

	return true;
}

//-----------------------------------//

bool IL_Image_Loader::checkErrors()
{
	ILenum err = ilGetError();

	if(err == IL_NO_ERROR)
		return false;

	Log::error("Error loading image via DevIL");
	//Log::error("Error loading image: '%s', iluErrorString(err));

	return true;
}

//-----------------------------------//

} // end namespace

#endif