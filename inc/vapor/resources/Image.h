/************************************************************************
*
* vaporEngine by triton © (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Engine.h"
#include "vapor/resources/Resource.h"

namespace vapor {
	namespace resources {

/**
 * Represents an image in the engine. An image usually holds 
 * uncompressed image data, stored in an pixel array in a given 
 * pixel format specified at creation time.
 */

class Image : public Resource
{

public:

	/**
	 * Gets the width of the image.
	 */
	const uint getWidth() const { return _width; }
	
	/**
	 * Gets the height of the image.
	 */
	const uint getHeight() const { return _height; }

private:

	// image width
	uint _width;

	// image height
	uint _height;
};

} } // end namespaces