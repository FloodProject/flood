/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/resources/Resource.h"

namespace vapor { namespace resources {

//-----------------------------------//

/**
 * Possible pixel formats for an image.
 */

namespace PixelFormat 
{
	enum Enum 
	{
		R8G8B8A8,
		R8G8B8,
		Unknown
	};

	const std::string getString(Enum e);
}

//-----------------------------------//

/**
 * Represents an image in the engine. An image usually holds uncompressed 
 * image data, stored in an pixel array in a given pixel format specified 
 * at creation time.
 *
 * The image data cannot be directly modified as is. Later we could add a
 * interface for locking the data buffer to be able to directly modify it.
 * (TODO: getBuffer? does it cause any problems?)
 */

class VAPOR_API Image : public Resource
{
public:

	Image(const ushort width, const ushort height, PixelFormat::Enum pf);
	virtual ~Image();

	// Gets the width of the image.	
	const ushort getWidth() const { return width; }
	
	// Gets the height of the image.
	const ushort getHeight() const { return height; }

	// Returns the pixel format of the image
	PixelFormat::Enum getPixelFormat() const { return pixelFormat; }

	// Gets the buffer containing the image data.
	const std::vector<byte>& getBuffer() const { return buffer; };

	// Change the buffer containing the image data.
	void setBuffer(const std::vector<byte>& data) { buffer = data; }

	// Return the proper resource group for this resource.
	virtual ResourceGroup::Enum getResourceGroup() const;

	// Prints image information to the log.
	void log() const;

private:

	// image width
	ushort width;

	// image height
	ushort height;

	// pixel format
	PixelFormat::Enum pixelFormat;

	// holds the image data
	std::vector<byte> buffer;
};

//-----------------------------------//

TYPEDEF_RESOURCE_POINTER_FROM_TYPE( Image );

//-----------------------------------//

} } // end namespaces