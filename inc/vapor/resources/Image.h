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

namespace PixelFormat 
{
	enum Enum 
	{
		R8G8B8A8 = 0,
		R8G8B8,
		Depth,
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

	Image();

	// Gets/sets the width of the image.
	IMPLEMENT_ACESSOR(Width, const ushort, width)
	
	// Gets/sets the height of the image.
	IMPLEMENT_ACESSOR(Height, const ushort, height)

	// Gets/sets the pixel format of the image.
	IMPLEMENT_ACESSOR(PixelFormat, PixelFormat::Enum, pixelFormat)

	// Gets/sets the buffer containing the image data.
	IMPLEMENT_ACESSOR(Buffer, const std::vector<byte>&, buffer)

	// Return the proper resource group for this resource.
	IMPLEMENT_GETTER(ResourceGroup, ResourceGroup::Enum, ResourceGroup::Images)

	// Saves the image contents to a file.
	void save( const std::string& filename );

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

TYPEDEF_RESOURCE_POINTER_FROM_TYPE( Image );

//-----------------------------------//

} // end namespace