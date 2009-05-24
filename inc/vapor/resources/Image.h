/************************************************************************
*
* vaporEngine by triton © (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"
#include "vapor/resources/Resource.h"
#include "vapor/resources/ResourceManager.h"

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

	// Gets the width of the image.	
	const uint getWidth() const { return width; }
	
	// Gets the height of the image.
	const uint getHeight() const { return height; }

	// Return the proper Images resource group
	virtual ResourceGroup::Enum getResourceGroup() { return ResourceGroup::Images; }

private:

	// image width
	uint width;

	// image height
	uint height;
};

} } // end namespaces