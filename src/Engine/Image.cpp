/************************************************************************
*
* vaporEngine by triton © (2008-2009)
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



} } // end namespaces