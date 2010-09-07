/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/resources/Resource.h"

namespace vapor {

//-----------------------------------//

/**
 * Possible pixel formats for an image.
 */

struct PixelFormat 
{
	enum Enum 
	{
		R8G8B8A8 = 0,
		R8G8B8,
		Depth,
		Unknown
	};

	DECLARE_ENUM()
};

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
	DECLARE_CLASS_()

public:

	Image();
	Image(ushort width, ushort height, PixelFormat::Enum format);

	// Gets/sets the width of the image.
	ACESSOR(Width, const ushort, width)
	
	// Gets/sets the height of the image.
	ACESSOR(Height, const ushort, height)

	// Gets/sets the pixel format of the image.
	ACESSOR(PixelFormat, PixelFormat::Enum, pixelFormat)

	// Gets/sets the buffer containing the image data.
	ACESSOR(Buffer, const std::vector<byte>&, buffer)

	// Return the proper resource group for this resource.
	GETTER(ResourceGroup, ResourceGroup::Enum, ResourceGroup::Images)

	// Saves the image contents to a file.
	void save( const std::string& filename );

	// Prints image information to the log.
	void log() const;

private:

	// Image width.
	ushort width;

	// Image height.
	ushort height;

	// Pixel format.
	PixelFormat::Enum pixelFormat;

	// Image data.
	std::vector<byte> buffer;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Image );

//-----------------------------------//

} // end namespace