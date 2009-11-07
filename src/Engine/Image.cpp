/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

#include "vapor/resources/Image.h"

namespace vapor {
	namespace resources {

//-----------------------------------//

namespace PixelFormat
{
	const std::string getString(Enum e) 
	{
		switch(e) 
		{
		case R8G8B8A8:
			return "R8G8B8A8";
		default:
			return "";
		}
	}
}

//-----------------------------------//

Image::Image(const ushort w, const ushort h, PixelFormat::Enum pf)
	: width(w), height(h), pixelFormat(pf) 
{
}

//-----------------------------------//

Image::~Image()
{
	//delete buffer;
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