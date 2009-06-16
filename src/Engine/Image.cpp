/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/resources/Image.h"

namespace vapor {
	namespace resources {

//-----------------------------------//

namespace PixelFormat
{
	string getString(Enum e) 
	{
		switch(e) 
		{
		case R8G8B8A8:
			return "R8G8B8A8";
		default:
			return nullptr;
		}
	}
}

//-----------------------------------//

Image::Image(const uint w, const uint h, PixelFormat::Enum pf)
	: width(w), height(h), pixelFormat(pf), dataBuffer(nullptr) 
{
}

//-----------------------------------//

Image::~Image()
{
}

//-----------------------------------//

void Image::log() const
{
	info("Image", "Image has pixel format '%s' and size %dx%d", 
		PixelFormat::getString(getPixelFormat()).c_str(), 
		getWidth(), getHeight());
}

//-----------------------------------//

} } // end namespaces