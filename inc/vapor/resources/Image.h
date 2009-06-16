/************************************************************************
*
* vaporEngine (2008-2009)
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
 * Possible pixel formats for an image.
 */

namespace PixelFormat 
{
	enum Enum 
	{
		R8G8B8A8,
	};

	string getString(Enum e);
}

/**
 * Represents an image in the engine. An image usually holds uncompressed 
 * image data, stored in an pixel array in a given pixel format specified 
 * at creation time.
 *
 * The image data cannot be directly modified as is. Later we could add a
 * interface for locking the data buffer to be able to directly modify it.
 * (TODO: getBuffer? does it cause any problems?)
 */

class Image : public Resource
{
public:

	Image(const uint width, const uint height, PixelFormat::Enum pf);
	virtual ~Image();

	// Gets the width of the image.	
	const uint getWidth() const { return width; }
	
	// Gets the height of the image.
	const uint getHeight() const { return height; }

	// Returns the pixel format of the image
	PixelFormat::Enum getPixelFormat() const { return pixelFormat; }

	// Gets a pointer to the buffer containing the data.
	void* getBuffer() const { return dataBuffer; };

	void setBuffer(void* data) { dataBuffer = data; }

	// Return the proper Images resource group
	virtual ResourceGroup::Enum getResourceGroup() { return ResourceGroup::Images; }

private:

	// image width
	uint width;

	// image height
	uint height;

	// pixel format
	PixelFormat::Enum pixelFormat;

	// holds the image data
	void* dataBuffer;
};

} } // end namespaces